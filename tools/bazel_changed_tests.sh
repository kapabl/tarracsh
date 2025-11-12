#!/usr/bin/env bash
# Determines the set of Bazel tests impacted by the given source files.
# Usage: tools/bazel_changed_tests.sh path/to/file1 [path/to/file2 ...]

set -euo pipefail

usage() {
  cat <<'EOF'
Usage: bazel_changed_tests.sh <file> [<file>...]

Given one or more files, this script:
  1. Maps each file to the Bazel targets that own it via `bazel query kind('rule', rdeps(...))`.
  2. Finds all Bazel tests that (reverse) depend on any of those targets.
The resulting test labels are printed to stdout, one per line.
EOF
}

if [[ $# -eq 0 ]]; then
  usage
  exit 1
fi

case "$1" in
  -h|--help)
    usage
    exit 0
    ;;
esac

if [[ -z "${BAZEL_BIN:-}" ]]; then
  if command -v bazel >/dev/null 2>&1; then
    BAZEL_BIN=$(command -v bazel)
  elif [[ -x /usr/local/bin/bazel ]]; then
    BAZEL_BIN=/usr/local/bin/bazel
  else
    echo "bazel command not found; install it or set BAZEL_BIN to its path" >&2
    exit 1
  fi
fi

export BAZEL_BIN

WORKSPACE_ROOT=""
if ! WORKSPACE_ROOT=$("$BAZEL_BIN" info workspace 2>/dev/null); then
  if ! WORKSPACE_ROOT=$(git rev-parse --show-toplevel 2>/dev/null); then
    echo "Unable to determine workspace root via 'bazel info workspace' or git" >&2
    exit 1
  fi
fi
WORKSPACE_ROOT=$(realpath "$WORKSPACE_ROOT")

normalize_path() {
  local input="$1"
  local candidate
  if [[ "$input" = /* ]]; then
    candidate="$input"
  else
    candidate="$WORKSPACE_ROOT/$input"
  fi

  local abs
  if ! abs=$(realpath -m -- "$candidate"); then
    echo "Failed to resolve path '$input'" >&2
    return 1
  fi

  case "$abs" in
    "$WORKSPACE_ROOT" | "$WORKSPACE_ROOT"/*)
      ;;
    *)
      echo "Path '$input' (resolved to '$abs') is outside the workspace $WORKSPACE_ROOT" >&2
      return 1
      ;;
  esac

  local rel="${abs#$WORKSPACE_ROOT/}"
  if [[ -z "$rel" ]]; then
    echo "Path '$input' resolves to the workspace root; please specify a file" >&2
    return 1
  fi

  printf '%s\n' "$rel"
}

declare -a workspace_paths=()
for file in "$@"; do
  if ! rel_path=$(normalize_path "$file"); then
    exit 1
  fi
  workspace_paths+=("$rel_path")
done

if [[ ${#workspace_paths[@]} -eq 0 ]]; then
  echo "No valid files provided" >&2
  exit 1
fi

declare -a owner_targets=()
declare -A seen_owner_targets=()

path_to_label() {
  local rel="$1"
  local pkg_part file_part
  if [[ "$rel" == */* ]]; then
    pkg_part="${rel%/*}"
    file_part="${rel##*/}"
  else
    pkg_part=""
    file_part="$rel"
  fi

  if [[ -z "$file_part" ]]; then
    echo "Unable to derive file name from path '$rel'" >&2
    return 1
  fi

  if [[ -z "$pkg_part" ]]; then
    printf '//:%s\n' "$file_part"
  else
    printf '//%s:%s\n' "$pkg_part" "$file_part"
  fi
}

for rel_path in "${workspace_paths[@]}"; do
  echo "Finding owners for $rel_path..." >&2

  if ! file_label=$(path_to_label "$rel_path"); then
    echo "  Unable to convert path '$rel_path' into a Bazel label (skipping)" >&2
    continue
  fi

  query_expr="kind('rule', rdeps(//..., ${file_label}, 1))"
  if ! mapfile -t owners < <("$BAZEL_BIN" query --noshow_progress --keep_going "$query_expr" 2>/dev/null); then
    owners=()
  fi

  if [[ ${#owners[@]} -eq 0 ]]; then
    echo "  No Bazel target owns '$rel_path' (skipping)" >&2
    continue
  fi

  for owner in "${owners[@]}"; do
    if [[ -n "$owner" && -z "${seen_owner_targets[$owner]+x}" ]]; then
      seen_owner_targets["$owner"]=1
      owner_targets+=("$owner")
    fi
  done
done

if [[ ${#owner_targets[@]} -eq 0 ]]; then
  echo "No owning targets found for the provided files." >&2
  exit 0
fi

mapfile -t owner_targets < <(printf '%s\n' "${owner_targets[@]}" | sort -u)

owners_expr=""
if [[ ${#owner_targets[@]} -gt 0 ]]; then
  owners_expr="set(${owner_targets[*]})"
fi

query="tests(rdeps(//..., ${owners_expr}))"
echo "Querying Bazel for affected tests..." >&2
"$BAZEL_BIN" query --noshow_progress --keep_going "$query"

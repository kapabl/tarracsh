#!/usr/bin/env bash
set -euo pipefail

[[ $# -gt 0 ]] || { echo "Usage: $0 <file> [<file>...]" >&2; exit 1; }
[[ "$1" = "-h" || "$1" = "--help" ]] && { echo "Usage: $0 <file> [<file>...]"; exit 0; }

BAZEL=${BAZEL_BIN:-bazel}
declare -A tests=()

for path in "$@"; do
  [[ "$path" != /* ]] || { echo "Paths must be workspace-relative: $path" >&2; exit 1; }
  rel=${path#./}
  pkg=${rel%/*}
  [[ "$pkg" == "$rel" ]] && label="//:${rel}" || label="//${pkg}:${rel##*/}"

  while IFS= read -r owner; do
    [[ -n "$owner" ]] || continue
    while IFS= read -r test; do
      [[ -n "$test" ]] || continue
      tests[$test]=1
    done < <("$BAZEL" query --noshow_progress --keep_going "kind('.*_test rule', rdeps(//..., ${owner}))")
  done < <("$BAZEL" query --noshow_progress --keep_going "kind('rule', rdeps(//..., ${label}, 1))")
done

[[ ${#tests[@]} -eq 0 ]] && exit 0
printf '%s\n' "${!tests[@]}" | sort

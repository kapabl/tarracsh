#!/usr/bin/env bash
# Runs git diff to discover changed files and feeds them into bazel_changed_tests.sh.
# Usage:
#   tools/git_diff_changed_tests.sh                 # uses unstaged + staged changes
#   tools/git_diff_changed_tests.sh --cached        # staged only
#   tools/git_diff_changed_tests.sh origin/main...  # custom diff args

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKSPACE_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
CHANGED_TESTS_SCRIPT="$SCRIPT_DIR/bazel_changed_tests.sh"

usage() {
  cat <<'EOF'
Usage: git_diff_changed_tests.sh [<git-diff-args>...]

Runs `git diff --name-only --diff-filter=ACMR <args>` to determine which files
have changed, then invokes `tools/bazel_changed_tests.sh` with those files.

Examples:
  git_diff_changed_tests.sh                    # unstaged + staged changes
  git_diff_changed_tests.sh --cached           # staged changes only
  git_diff_changed_tests.sh origin/main...HEAD # compare branch vs origin/main
EOF
}

case "${1:-}" in
  -h|--help)
    usage
    exit 0
    ;;
esac

if [[ ! -x "$CHANGED_TESTS_SCRIPT" ]]; then
  echo "Missing executable $CHANGED_TESTS_SCRIPT" >&2
  exit 1
fi

cd "$WORKSPACE_ROOT"

diff_args=("$@")
mapfile -t changed_files < <(git diff --name-only --diff-filter=ACMR "${diff_args[@]}" | sed '/^\s*$/d')

if [[ ${#changed_files[@]} -eq 0 ]]; then
  echo "No changed files detected via git diff ${diff_args[*]:-(working tree)}" >&2
  exit 0
fi

"$CHANGED_TESTS_SCRIPT" "${changed_files[@]}"

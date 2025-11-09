#!/bin/bash
set -euo pipefail

echo "Legacy builds now delegate to Bazel." >&2

if [[ $# -eq 0 ]]; then
  set -- "//src:tarracsh"
fi

echo "bazel build $*" >&2
bazel build "$@"

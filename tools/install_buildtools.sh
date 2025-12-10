#!/usr/bin/env bash
# Installs Bazel build tools: buildifier, buildozer, and gazelle.
# Requires Go 1.19+ in PATH. Binaries are placed under GOPATH/bin (defaults to ~/go/bin).
set -euo pipefail

if ! command -v go >/dev/null 2>&1; then
  echo "ERROR: Go is required but not found in PATH." >&2
  exit 1
fi

export GOPATH="${GOPATH:-$HOME/go}"
export PATH="$GOPATH/bin:$PATH"

echo "==> Installing buildifier, buildozer, gazelle via go install"
go install github.com/bazelbuild/buildtools/buildifier@latest
go install github.com/bazelbuild/buildtools/buildozer@latest
go install github.com/bazelbuild/bazel-gazelle/cmd/gazelle@latest

echo "Done. Ensure \$GOPATH/bin (current: $GOPATH/bin) is on your PATH."

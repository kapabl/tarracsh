#!/usr/bin/env bash
# Wrapper to ensure vcpkg is provisioned before invoking buck2.
# Uses the fixed paths from tools/vcpkg/setup-vcpkg.sh.
set -euo pipefail

PREFIX="/opt/vcpkg_installed/x64-linux"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

if [[ ! -d "${PREFIX}/include" ]]; then
  echo "==> vcpkg not found at ${PREFIX}; bootstrapping..."
  sudo bash "${SCRIPT_DIR}/setup-vcpkg.sh"
fi

exec buck2 "$@"

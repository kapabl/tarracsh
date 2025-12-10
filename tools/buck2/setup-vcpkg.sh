#!/usr/bin/env bash
# Minimal vcpkg bootstrap for this repo used by Buck2 workflows.
# Installs vcpkg into /opt/vcpkg and the triplet output into /opt/vcpkg_installed/x64-linux.
# Requires write access to /opt (run with sudo if needed).
set -euo pipefail

VCPKG_ROOT="/opt/vcpkg"
VCPKG_PREFIX="/opt/vcpkg_installed"
TRIPLET="x64-linux"

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/../.." && pwd)"
manifest="${repo_root}/vcpkg.json"

mkdir -p "${VCPKG_ROOT}"
mkdir -p "${VCPKG_PREFIX}"

if [[ ! -x "${VCPKG_ROOT}/vcpkg" ]]; then
  echo "==> Cloning and bootstrapping vcpkg into ${VCPKG_ROOT}"
  rm -rf "${VCPKG_ROOT}"
  git clone https://github.com/microsoft/vcpkg.git "${VCPKG_ROOT}"
  (cd "${VCPKG_ROOT}" && ./bootstrap-vcpkg.sh)
fi

echo "==> Installing manifest ${manifest} -> ${VCPKG_PREFIX} (triplet=${TRIPLET})"
"${VCPKG_ROOT}/vcpkg" install \
  --triplet "${TRIPLET}" \
  --x-manifest-root "${manifest%/vcpkg.json}" \
  --x-install-root "${VCPKG_PREFIX}" \
  --clean-after-build

echo "Done. vcpkg prefix at ${VCPKG_PREFIX}"

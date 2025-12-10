#!/usr/bin/env bash
# Sets up Buck2 and C/C++ build prerequisites on CentOS Stream 9 (WSL2-friendly).
# - Installs compilers and common build tools via dnf.
# - Runs basic version checks at the end.
set -euo pipefail

INSTALL_DIR="${INSTALL_DIR:-/usr/local/bin}"

echo "==> Installing toolchain and build deps via dnf"
dnf -y groupinstall "Development Tools"
dnf -y install \
  git curl unzip zip tar python3 python3-pip ninja-build pkgconfig cmake \
  clang llvm zstd

tmpdir="$(mktemp -d)"
# Hardcoded to the latest Buck2 release asset (glibc).
url="https://github.com/facebook/buck2/releases/download/2025-12-01/buck2-x86_64-unknown-linux-gnu.zst"
echo "==> Downloading Buck2 from ${url}"
if ! curl -fL "${url}" -o "${tmpdir}/buck2_asset"; then
  echo "ERROR: Buck2 download failed: ${url}" >&2
  exit 1
fi
zstd -d -c "${tmpdir}/buck2_asset" > "${tmpdir}/buck2"
chmod +x "${tmpdir}/buck2"
install -m 0755 "${tmpdir}/buck2" "${INSTALL_DIR}/buck2"

echo "==> Version checks"
g++ --version | head -n1 || true
clang++ --version | head -n1 || true
cmake --version | head -n1 || true
ninja --version || true
python3 --version || true
buck2 --version || true

echo "Done."

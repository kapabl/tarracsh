#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT_DIR="${1:-$SCRIPT_DIR/build-graphs}"
mkdir -p "$OUT_DIR"

FULL_DOT="$OUT_DIR/build-graph-full.dot"
FULL_GML="$OUT_DIR/build-graph-full.gml"
TARGET_DOT="$OUT_DIR/build-graph-targets.dot"
TARGET_GML="$OUT_DIR/build-graph-targets.gml"
CONVERTER="$SCRIPT_DIR/convert_dot_to_gml.py"


FULL_QUERY="deps(set(//src:tarracsh //src/interfaces/cfdiff:cfdiff))"
TARGET_QUERY="kind(\"rule\", ${FULL_QUERY})"

bazel query "$FULL_QUERY" --output graph > "$FULL_DOT"
bazel query "$TARGET_QUERY" --output graph > "$TARGET_DOT"

if ! command -v dot >/dev/null 2>&1; then
  echo "Warning: 'dot' not found; skipping GML creation" >&2
else
  python3 "$CONVERTER" "$FULL_DOT" "$FULL_GML"
  python3 "$CONVERTER" "$TARGET_DOT" "$TARGET_GML"
  echo "GML created: $FULL_GML"
  echo "GML created: $TARGET_GML"
fi

echo "Full graph:   $FULL_DOT"
echo "Target graph: $TARGET_DOT"

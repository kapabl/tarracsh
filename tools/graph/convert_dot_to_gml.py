#!/usr/bin/env python3
import re
import subprocess
import sys
from pathlib import Path
from typing import Iterable, List


def sanitize_dot_lines(lines: Iterable[str]) -> List[str]:
    cleaned: List[str] = []
    for raw in lines:
        line = raw.rstrip('\n')
        line = re.sub(r'\\n([^\"]*)', r'|\1', line)
        line = re.sub(r'\.\.\.and \d+ more items', '', line)
        if re.match(r'^\s*\[label=', line):
            continue
        line = line.rstrip()
        if not line:
            cleaned.append(line)
            continue
        if (line.strip() not in {'digraph mygraph {', '{', '}'}) and not line.rstrip().endswith(';'):
            line += ';'
        cleaned.append(line)
    return cleaned


def sanitize_label(label: str) -> str:
    replacements = {
        '"': "'",
        '@@': 'atat_',
        '@': 'at_',
        '//': '',
    }
    for old, new in replacements.items():
        label = label.replace(old, new)
    label = re.sub(r'[^A-Za-z0-9_:\-\.\|/]', '_', label)
    return label


def convert(cleaned_dot: Path, gml_path: Path) -> None:
    proc = subprocess.run(["dot", "-Tplain", str(cleaned_dot)], capture_output=True, text=True, check=True)
    lines = proc.stdout.splitlines()
    nodes = {}
    edges = []
    next_id = 0

    def get_id(name: str) -> int:
        nonlocal next_id
        if name not in nodes:
            nodes[name] = {'id': next_id, 'label': sanitize_label(name)}
            next_id += 1
        return nodes[name]['id']

    for raw in lines:
        parts = raw.strip().split()
        if not parts:
            continue
        kind = parts[0]
        if kind == 'node':
            name = parts[1]
            label = parts[6] if len(parts) > 6 else name
            node_id = get_id(name)
            nodes[name]['label'] = sanitize_label(label)
        elif kind == 'edge':
            tail, head = parts[1], parts[2]
            src = get_id(tail)
            dst = get_id(head)
            edges.append((src, dst))

    with gml_path.open('w', encoding='utf-8') as fh:
        fh.write('graph [\n')
        for data in nodes.values():
            label = data['label']
            fh.write(f'  node [ id {data["id"]} label "{label}" name "{label}" ]\n')
        for src, dst in edges:
            fh.write(f'  edge [ source {src} target {dst} ]\n')
        fh.write(']\n')


def main() -> None:
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <input.dot> <output.gml>", file=sys.stderr)
        sys.exit(1)

    dot_path = Path(sys.argv[1])
    gml_path = Path(sys.argv[2])
    cleaned_lines = sanitize_dot_lines(dot_path.read_text().splitlines())
    cleaned_path = dot_path.with_suffix(dot_path.suffix + '.sanitized')
    cleaned_path.write_text('\n'.join(cleaned_lines) + '\n')
    convert(cleaned_path, gml_path)
    cleaned_path.unlink(missing_ok=True)


if __name__ == '__main__':
    main()

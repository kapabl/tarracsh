# cfdiff - Class file diff

`cfdiff` is a lightweight CLI utility for spotting structural differences between two Java class files. It parses each class file, compares the class name, fields, methods, and selected attributes, then reports any mismatches with color‑coded output.

When changing compiler versions of kotlin and java, it is useful to see how many things have change from binary to binary. Scala not so useful as the binaries from one version to another are not compatible.

## Build

1. **Dependencies**
   - GCC 13 (or any C++20‑capable compiler)
   - CMake
   - Ninja
2. **Repository layout**

repos/
tarracsh/
vcpkg/


3. **Clone**
```bash
git clone git@github.com:kapabl/tarracsh.git
git clone https://github.com/microsoft/vcpkg.git
Build

cd tarracsh/cfdiff
./build-all.sh
Usage
cfdiff [options] <left.class> <right.class>
Options

--ignore-classname Ignore differences in the class name.

--ignore-attributes=<list> Comma‑separated list of attributes to skip (e.g., --ignore-attributes=Code,SourceFile).

-p, --public-only Compare only public members (default: yes).

--version Show tool version.

--help-all Display full help.

Example

./cfdiff --ignore-classname -p MyClassA.class MyClassB.class
If there are no differences, the tool prints “No differences found”; otherwise, it reports the number of mismatches and highlights each discrepancy.

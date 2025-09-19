## Tarracsh

Tarracsh is a C++20 command‑line tool for analyzing Java bytecode. It parses .class files or JARs, computes digests, builds call graphs, and can generate HTML navigation for constant‑pool entries

Features
Class file parser – parse directories, JARs, or individual class files; print parse output, constant pool, filter entries, or generate an HTML navigation page

Public digest – create or compare digests of class files and JARs, run standalone or in client/server mode, and produce diff reports

Call graph – build a call‑graph database from bytecode (server/client modes currently placeholders)

Global options – output directory, logging, profiler output, and file timestamp checks configurable for all commands

Building
The project uses CMake with a C++20 compiler and depends on libraries such as ICU, antlr4, CLI11, gRPC, inja, libsodium, libzippp, and fmt. A helper script fetches dependencies via vcpkg and builds the executable:

./build.sh

Usage
Run tarracsh <command> [options].

Parse
tarracsh parse --input <path> [--print-class-parse] [--print-cpool] [--cpool-filter type] [--html-nav]
Parses the target and optionally prints or renders constant‑pool information.

Public Digest
tarracsh public-digest --input <path> [--diff] [--server|--port p --host h]
Generates or compares digest databases, optionally communicating with a gRPC server.

Call Graph
tarracsh call-graph --input <path>
Builds a call‑graph database. Server and client modes are placeholders and currently emit a “not implemented” message.

Global flags like --output-dir, --output-log-file, --print-profiler, and --use-file-timestamp apply to all commands. Use --version or --help-all for version info and comprehensive help.

### Query language

Commands that support the `--query` option accept a compact SQL-like language defined in
[`KapaQuery.g4`](src/infrastructure/db/query/grammar/KapaQuery.g4).

* Statements:
  * `schema` prints the database tables with their columns.
  * `list <table>` or `select <table>` optionally followed by a `where` clause returns rows from the
    given table.
* Filters combine a column name, an operator, and a quoted string literal. Supported operators are:
  * `=` and `!=` for equality / inequality comparisons.
  * `^*` (`not ^*`) for starts-with (and its negation).
  * `*^` (`not *^`) for ends-with (and its negation).
  * `*^*` for regular-expression matches.
* String literals can use single (`'value'`) or double (`"value"`) straight quotes; double the quote
  character to embed it inside the literal. Smart quotes copied from rich text, such as `“value”` or
  `“value″`, are also accepted.
* Logical composition uses parentheses together with `and` / `or`, e.g.
  `(method ^* "com/example" and (owner = 'ClassName' or owner = 'OtherClass'))`.

Combine `--query` with `--display-raw` to print unformatted column values when needed.

Project Status
Call‑graph server/client functionality is marked TODO and not yet implemented.

https://en.wikipedia.org/wiki/Siegbert_Tarrasch

![image](https://github.com/kapabl/tarracsh/assets/105734562/7f6a9681-eaf4-4775-9d3b-4dd7275017f2)

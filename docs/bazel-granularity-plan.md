# Bazel Dependency Granularity Plan

## Goals
- Break the current monolithic `//src:tarracsh` target into directory-scoped libraries.
- Map existing tests to those new libraries so Bazel knows which code each test covers.
- Provide a scriptable query to select only the tests impacted by a given set of changed files.

## Execution Steps
1. **Inventory directories** *(done)*: list the top-level subdirectories under `src/` (app, domain, infrastructure, resources) and the existing tests under `test/`.
2. **Refactor BUILD files**:
   1. **Per-directory packages** *(in progress)* – add a `BUILD.bazel` under each major `src/...` directory plus its matching `test/...` directory. Each package exports the `cc_library` or `cc_test` that owns just that subtree.  
      - ✅ Infrastructure side now split into `app`, `filesystem`, `log`, `profiling`, `db`, and `string` packages, with matching `test/src/infrastructure/{db,filesystem}` `cc_test`s aggregated via a suite.
      - ✅ Domain `db/` now has its own `cc_library` and `test/src/domain/db` owns a dedicated `db_tests`, with the legacy `domain_tests` target converted to a suite.
   2. Keep a slim `//src:tarracsh` binary target that just depends on these libraries (already implemented).
3. **Align tests** *(started)*:
   1. Ensure each `test/src/<dir>` tree depends on the counterpart `//src/<dir>:<target>`.
   2. Split overly broad tests if needed so the dependency graph stays precise.
4. **Automate impact analysis** *(todo)*:
   1. Add a helper script (e.g. `tools/bazel_changed_tests.sh`) that takes a list of changed files, maps them to owning targets via `bazel query owner(...)`, and runs `bazel query 'tests(rdeps(//..., set(<targets>)))'`.
   2. Document how to integrate the script into CI.
5. **Validation** *(continuous)*:
   1. Run `bazel build //src:tarracsh` and `bazel test //test/...` after each directory split.
   2. Smoke-test the script with a simulated change (e.g. touch a file) to confirm it returns the expected minimal test set.

We will apply these steps iteratively per directory so we always keep the build in a working state.

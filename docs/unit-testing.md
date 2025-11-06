Unit Testing & Coverage Plan
============================

Goal: establish a repeatable unit-testing workflow that delivers at least 90% line coverage across the codebase while keeping future regressions out.

1. Select & Install Test Framework
   - Use `vcpkg` to install the chosen framework (e.g., `catch2` or `gtest`) and record the dependency in `vcpkg.json`.
   - Add the framework to the project toolchain via `CMakeLists.txt`, exporting include directories and linking targets for reuse.

2. Enable Coverage-Friendly Build Configuration
   - Create a dedicated coverage build type (`Coverage`) or extend `Debug` to inject `-O0 -g --coverage` (GCC) or `-fprofile-instr-generate -fcoverage-mapping` (Clang).
   - Update all unit-test targets to propagate the coverage flags and ensure artifacts are emitted into `build/coverage`.

3. Audit Codebase & Define Test Surface
   - Enumerate modules (analyzer, app context, server request handling, utilities) and map their public seams.
   - Document high-risk areas (parsing, concurrency, configuration loading) and note external integrations that require mocks/stubs.

4. Build Initial Test Harness
   - Create a `tests/` subtree with a `CMakeLists.txt` that mirrors production include paths and compiles per-module test executables.
   - Add reusable fixtures, mocks, and helper utilities to isolate dependencies and enforce deterministic behavior.

5. Author Targeted Unit Suites
   - Start with complex or brittle code paths; write positive, negative, and edge-case scenarios.
   - Capture known bug regressions in dedicated tests and require new features to ship with matching coverage.

6. Integrate Coverage Reporting
   - Automate `ctest` execution followed by `lcov && genhtml` (or `llvm-profdata`/`llvm-cov`) to produce HTML reports in `build/coverage/report`.
   - Gate the workflow with a coverage threshold (fail if global coverage < 90% or per-module coverage slips below agreed floors).

7. Automate & Monitor
   - Wire unit-test + coverage targets into CI (GitHub Actions, GitLab CI, etc.), caching `vcpkg` artifacts to keep builds fast.
   - Publish coverage reports as CI artifacts; review trends periodically to keep technical debt in check.

8. Maintain Discipline
   - Add contribution guidelines that require tests for new code paths and coverage parity before merge.
   - Schedule periodic audits to refactor brittle tests, remove flakes, and expand coverage for new components.

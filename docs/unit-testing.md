Unit Testing & Coverage Plan
============================

Goal: establish a repeatable unit-testing workflow that delivers at least 90% line coverage across the codebase while keeping future regressions out.

1. Framework & Dependency Management
   - GoogleTest is the canonical framework; it is declared in `vcpkg.json` and resolved automatically during CMake configure.
   - All production sources build into `tarracsh_core`, which is reused by the CLI executable and the `tarracsh_tests` binary.

2. Coverage-Friendly Build Configuration
   - Toggle instrumentation with `-DENABLE_COVERAGE=ON`. GCC/Clang builds receive `-O0 -g --coverage` and link with `--coverage`.
   - Coverage artefacts are emitted into `${build}/coverage`, keeping normal release builds unaffected.

3. Audit Codebase & Define Test Surface
   - Enumerate modules (analyzer, app context, server request handling, utilities) and map their public seams.
   - Document high-risk areas (parsing, concurrency, configuration loading) and note external integrations that require mocks/stubs.

4. Build Initial Test Harness
   - Test sources live under `test/src/**`. Every new suite should link against `tarracsh_core` plus any mocks or fixtures it needs.
   - Favor thin seams (e.g., digest helpers, string utilities) to keep tests fast and deterministic.

5. Author Targeted Unit Suites
   - Start with complex or brittle code paths; write positive, negative, and edge-case scenarios.
   - Capture known bug regressions in dedicated tests and require new features to ship with matching coverage.

6. Integrate Coverage Reporting
   - With coverage enabled, run `cmake --build <build> --target coverage` to execute the suite, capture `lcov` data, and emit HTML into `${build}/coverage/html`.
   - Review the generated report and raise alarms when global line coverage dips below the 90% target.

7. Automate & Monitor
   - Wire unit-test + coverage targets into CI (GitHub Actions, GitLab CI, etc.), caching `vcpkg` artifacts to keep builds fast.
   - Publish coverage reports as CI artifacts; review trends periodically to keep technical debt in check.

8. Maintain Discipline
   - Add contribution guidelines that require tests for new code paths and coverage parity before merge.
   - Schedule periodic audits to refactor brittle tests, remove flakes, and expand coverage for new components.

Quick Start
-----------

1. Configure: `cmake -S . -B cmake-build-debug -DENABLE_COVERAGE=ON`
2. Build tests: `cmake --build cmake-build-debug --target tarracsh_tests`
3. Run locally: `ctest --output-on-failure` (from the build directory)
4. Generate coverage: `cmake --build cmake-build-debug --target coverage`

Keep iterating on suites until the HTML report indicates ≥90% line coverage. Prioritise hot paths and bug-prone modules before expanding to low-risk code.

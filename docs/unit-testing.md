Unit Testing & Coverage Plan
============================

Goal: establish a repeatable unit-testing workflow that delivers at least 90% line coverage across the codebase while keeping future regressions out.

1. Framework & Dependency Management
   - GoogleTest is provided by Bazel via the `//:vcpkg_gtest_main` target, so no manual package installs are needed.
   - All production sources build into `//src:tarracsh_core`, which is reused by the CLI executable and the Bazel `cc_test` targets.

2. Coverage-Friendly Build Configuration
   - Bazel enables instrumentation via `bazel coverage`; pass `--combined_report=lcov` and `--instrumentation_filter=//src/...,//test/...` to focus on project code.
   - Coverage artefacts are emitted into `bazel-out/_coverage/_coverage_report.dat` and can be converted to HTML with `genhtml`.

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
   - Run `bazel coverage //test:tarracsh_tests --combined_report=lcov --instrumentation_filter=//src/...,//test/...` to execute the suite and emit `bazel-out/_coverage/_coverage_report.dat`.
   - Convert the LCOV file with `genhtml bazel-out/_coverage/_coverage_report.dat --output-directory coverage/html`, then review the generated report and raise alarms when global line coverage dips below the 90% target.

7. Automate & Monitor
   - Wire unit-test + coverage targets into CI (GitHub Actions, GitLab CI, etc.), caching `vcpkg` artifacts to keep builds fast.
   - Publish coverage reports as CI artifacts; review trends periodically to keep technical debt in check.

8. Maintain Discipline
   - Add contribution guidelines that require tests for new code paths and coverage parity before merge.
   - Schedule periodic audits to refactor brittle tests, remove flakes, and expand coverage for new components.

Quick Start
-----------

1. Build the CLI and libraries: `bazel build //src:tarracsh`
2. Run the suite: `bazel test //test:tarracsh_tests`
3. Collect coverage: `bazel coverage //test:tarracsh_tests --combined_report=lcov --instrumentation_filter=//src/...,//test/...`
4. Generate HTML: `genhtml bazel-out/_coverage/_coverage_report.dat --output-directory coverage/html`

Keep iterating on suites until the HTML report indicates ≥90% line coverage. Prioritise hot paths and bug-prone modules before expanding to low-risk code.

## Notes

- Do everything under the user bazel

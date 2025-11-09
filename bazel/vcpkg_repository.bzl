"""Custom repository rule to provision vcpkg-managed dependencies inside Bazel."""

def _dump_vcpkg_logs(ctx, buildtrees_root_path):
    script = """
root="{root}"
if [ -d "$root" ]; then
    find "$root" -type f \\( -name 'config-*.log' -o -name 'install-*.log' \\) | sort | while read -r file; do
        echo "===== $file ====="
        tail -n 80 "$file" || true
    done
fi
""".format(root = buildtrees_root_path)
    result = ctx.execute(
        ["/bin/bash", "-c", "{} || true".format(script)],
        working_directory = str(ctx.path(".")),
    )
    return result.stdout

def _run(ctx, args, working_directory, env = None, on_failure = None, timeout = None):
    exec_kwargs = {"working_directory": str(working_directory)}
    if env != None:
        exec_kwargs["environment"] = env
    if timeout:
        exec_kwargs["timeout"] = timeout
    result = ctx.execute(args, **exec_kwargs)
    if result.return_code:
        extra = on_failure() if on_failure else ""
        extra_msg = "\n\nCaptured vcpkg logs:\n{}".format(extra) if extra else ""
        fail("Command failed ({}): {}\nstdout:\n{}\nstderr:\n{}{}".format(
            result.return_code,
            " ".join(args),
            result.stdout,
            result.stderr,
            extra_msg,
        ))

def _download_vcpkg(ctx):
    strip_prefix = ctx.attr.strip_prefix or "vcpkg-{}".format(ctx.attr.vcpkg_commit)
    if ctx.attr.vcpkg_archive:
        archive_path = ctx.path(ctx.attr.vcpkg_archive)
        ctx.download_and_extract(
            url = "file://{}".format(archive_path),
            output = "vcpkg_src",
            stripPrefix = ctx.attr.strip_prefix or "vcpkg",
        )
        return ctx.path("vcpkg_src")

    if ctx.attr.vcpkg_url:
        download_kwargs = dict(
            url = ctx.attr.vcpkg_url,
            output = "vcpkg_src",
            stripPrefix = strip_prefix,
        )
        if ctx.attr.vcpkg_sha256:
            download_kwargs["sha256"] = ctx.attr.vcpkg_sha256
        ctx.download_and_extract(**download_kwargs)
        return ctx.path("vcpkg_src")

    vcpkg_root = ctx.path("vcpkg_src")
    _run(ctx, ["/bin/rm", "-rf", str(vcpkg_root)], working_directory = ctx.path("."))
    repo_url = ctx.attr.vcpkg_git_repository
    _run(
        ctx,
        ["git", "clone", repo_url, str(vcpkg_root)],
        working_directory = ctx.path("."),
    )
    _run(
        ctx,
        ["git", "-C", str(vcpkg_root), "checkout", ctx.attr.vcpkg_commit],
        working_directory = ctx.path("."),
    )
    return vcpkg_root

def _write_linker_response(ctx, triplet_path):
    script = """
set -euo pipefail
root="$(pwd)"
resp="{triplet}/lib/vcpkg_libs.rsp"
tmp="{triplet}/lib/vcpkg_libs.list"
rm -f "$resp" "$tmp"
for dir in {triplet}/lib {triplet}/lib64; do
    if [ -d "$dir" ]; then
        find "$dir" -type f -name '*.a' -print >> "$tmp"
    fi
done
if [ -f "$tmp" ]; then
    sort "$tmp" > "$tmp.sorted"
    mv "$tmp.sorted" "$tmp"
fi
{{
    echo "--start-group"
    if [ -f "$tmp" ]; then
        while read -r lib; do
            case "$lib" in
                */libzip.a) continue ;;
                *) printf '%s/%s\\n' "$root" "$lib" ;;
            esac
        done < "$tmp"
        if grep '/libzip\\.a$' "$tmp" > /dev/null; then
            grep '/libzip\\.a$' "$tmp" | while read -r libzip; do
                printf '%s/%s\\n' "$root" "$libzip"
            done
        fi
    fi
    echo "--end-group"
}} > "$resp"
rm -f "$tmp"
""".format(triplet = triplet_path)
    _run(
        ctx,
        ["/bin/bash", "-c", script],
        working_directory = ctx.path("."),
    )

def _write_manifest(ctx, manifest_dir):
    manifest_content = ctx.read(ctx.attr.manifest)
    ctx.file("{}/vcpkg.json".format(manifest_dir), manifest_content)

def _vcpkg_repository_impl(ctx):
    vcpkg_root = _download_vcpkg(ctx)
    bootstrap = vcpkg_root.get_child("bootstrap-vcpkg.sh")
    _run(
        ctx,
        ["/bin/chmod", "+x", str(bootstrap)],
        working_directory = vcpkg_root,
    )
    env = {
        "VCPKG_KEEP_BUILDTREES": "1",
    }
    _run(
        ctx,
        ["/bin/bash", str(bootstrap), "-disableMetrics"],
        working_directory = vcpkg_root,
        env = env,
    )

    vcpkg_exe = vcpkg_root.get_child("vcpkg")
    manifest_dir = ctx.path("manifest")
    _run(ctx, ["/bin/mkdir", "-p", str(manifest_dir)], working_directory = ctx.path("."))
    _write_manifest(ctx, "manifest")

    install_root_path = ctx.path("installed")
    packages_root_path = ctx.path("packages")
    buildtrees_root_path = ctx.path("buildtrees")
    for directory in (install_root_path, packages_root_path, buildtrees_root_path):
        _run(ctx, ["/bin/mkdir", "-p", str(directory)], working_directory = ctx.path("."))

    install_triplet_rel = "installed/{}".format(ctx.attr.triplet)
    install_args = [
        str(vcpkg_exe),
        "install",
        "--triplet={}".format(ctx.attr.triplet),
        "--feature-flags=manifests",
        "--clean-after-build",
        "--disable-metrics",
        "--debug",
        "--x-install-root={}".format(install_root_path),
        "--x-packages-root={}".format(packages_root_path),
        "--x-buildtrees-root={}".format(buildtrees_root_path),
        "--x-manifest-root={}".format(manifest_dir),
    ]
    _run(
        ctx,
        install_args,
        working_directory = vcpkg_root,
        env = env,
        timeout = 3600,
        on_failure = lambda: _dump_vcpkg_logs(ctx, buildtrees_root_path),
    )
    _write_linker_response(ctx, install_triplet_rel)
    install_triplet_abs = str(ctx.path(install_triplet_rel))

    build_content = """
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "headers",
    hdrs = glob(
        [
            "{triplet}/include/**",
        ],
        allow_empty = False,
    ),
    includes = [
        "{triplet}/include",
        "{triplet}/include/antlr4-runtime",
    ],
)

cc_library(
    name = "runtime_libs",
    srcs = [],
    additional_linker_inputs = glob(
        [
            "{triplet}/lib/**/*.a",
            "{triplet}/lib64/**/*.a",
        ],
        allow_empty = True,
    ) + [
        "{triplet}/lib/vcpkg_libs.rsp",
    ],
    deps = [":headers"],
    linkopts = [
        "-pthread",
        "-ldl",
        "-lz",
        "-Wl,@{triplet_abs}/lib/vcpkg_libs.rsp",
    ],
)

cc_library(
    name = "gtest_main",
    srcs = [
        "{triplet}/lib/manual-link/libgtest_main.a",
        "{triplet}/lib/manual-link/libgmock_main.a",
    ],
    deps = [":headers"],
)
""".format(triplet = install_triplet_rel, triplet_abs = install_triplet_abs)
    ctx.file("BUILD.bazel", build_content)

vcpkg_repository = repository_rule(
    implementation = _vcpkg_repository_impl,
    attrs = {
        "manifest": attr.label(mandatory = True, allow_single_file = True),
        "triplet": attr.string(default = "x64-linux"),
        "vcpkg_commit": attr.string(mandatory = True),
        "vcpkg_archive": attr.label(allow_single_file = True),
        "vcpkg_git_repository": attr.string(default = "https://github.com/microsoft/vcpkg.git"),
        "vcpkg_sha256": attr.string(default = ""),
        "vcpkg_url": attr.string(default = ""),
        "strip_prefix": attr.string(default = ""),
    },
    local = False,
    configure = True,
)

def _vcpkg_extension_impl(module_ctx):
    repos_created = False
    for module in module_ctx.modules:
        for config in module.tags.config:
            if not module.is_root:
                fail("vcpkg configuration is only supported in the root module")
            repo_name = config.name if config.name else "vcpkg"
            vcpkg_repository(
                name = repo_name,
                manifest = config.manifest,
                triplet = config.triplet,
                vcpkg_commit = config.vcpkg_commit,
                vcpkg_archive = config.vcpkg_archive,
                vcpkg_git_repository = config.vcpkg_git_repository,
                vcpkg_sha256 = config.vcpkg_sha256,
                vcpkg_url = config.vcpkg_url,
                strip_prefix = config.strip_prefix,
            )
            repos_created = True

    if repos_created and hasattr(module_ctx, "extension_metadata"):
        return module_ctx.extension_metadata(reproducible = True)
    return None

vcpkg_extension = module_extension(
    implementation = _vcpkg_extension_impl,
    tag_classes = {
        "config": tag_class(
            attrs = {
                "name": attr.string(default = "vcpkg"),
                "manifest": attr.label(mandatory = True, allow_single_file = True),
                "triplet": attr.string(default = "x64-linux"),
                "vcpkg_commit": attr.string(mandatory = True),
                "vcpkg_archive": attr.label(allow_single_file = True),
                "vcpkg_git_repository": attr.string(default = "https://github.com/microsoft/vcpkg.git"),
                "vcpkg_sha256": attr.string(default = ""),
                "vcpkg_url": attr.string(default = ""),
                "strip_prefix": attr.string(default = ""),
            },
        ),
    },
)

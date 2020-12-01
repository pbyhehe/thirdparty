##############third party########################

package(default_visibility = ["//visibility:public"],)

licenses(["notice"])  # Apache 2.0

# check git repo version
genrule(
    name = "check_git_version",
    outs = ["check_git_version.txt"],
    visibility = ["//visibility:private"],
    cmd = "echo '1' > $@ && ./$(location check_git_version_sh)",
    tools = [":check_git_version_sh"],
)

sh_binary(
    name = "check_git_version_sh",
    srcs = ["check_repo_version.py",],
    visibility = ["//visibility:public",],
)

cc_library(
    name = "boost",
    hdrs = glob([
        "boost/include/**/*.hpp",
        "boost/include/**/*.h",
        "boost/include/**/*.ipp",
        "boost/include/**/**/*.hpp",
        "boost/include/**/**/*.h",
        "boost/include/**/**/**/*.hpp",
        "boost/include/**/**/**/*.h",
    ],
    exclude = ["boost/include/boost/test/*"],),
    srcs = glob(["boost/lib/**/*.a", "boost/lib/*.a",],
    exclude = ["boost/lib/*_test_*", "boost/lib/*_prg_exec_*"],),
    includes = ["boost/include",],
    deps = [":zlib", ":iconv"],
    visibility = ["//visibility:public",],
)

cc_library(
    name = "curl",
    hdrs = glob(["curl/include/**/*.h"]),
    srcs = ["curl/lib/libcurl.a",],
    includes = ["curl/include",],
    deps = [":zlib",],
    visibility = ["//visibility:public",],
)

cc_library(
    name = "iconv",
    srcs = ["iconv/lib/libiconv.a"],
    hdrs = glob(["iconv/include/*.h"]),
    visibility = ["//visibility:public"],
    includes = ["iconv/include"],
)

cc_library(
    name = "zlib",
    srcs = glob(["zlib/lib/*.a"]),
    hdrs = glob(["zlib/include/*.h"]),
    visibility = ["//visibility:public"],
    includes = ["zlib/include"],
)

cc_library(
    name = "msgpack",
    hdrs = glob(["msgpack/*.h",
                "msgpack/*.hpp",
                "msgpack/msgpack/*.h",
                "msgpack/msgpack/*.hpp",
                "msgpack/msgpack/type/*.h",
                "msgpack/msgpack/type/*.hpp",
                "msgpack/msgpack/type/tr1/*.h",
                "msgpack/msgpack/type/tr1/*.hpp"]),
    srcs = glob(["msgpack/lib/*.a"]),
    includes = ["msgpack/msgpack/","msgpack/"],
    visibility = [
        "//visibility:public",
    ],
    linkstatic = 1,
)

cc_library(
    name = "zookeeper",
    hdrs = glob(["zookeeper/include/*.h"]),
    srcs = ["zookeeper/lib/libzookeeper_mt.a",],
    includes = ["zookeeper/include",],
    visibility = ["//visibility:public",],
)

cc_library(
    name = "zkservice",
    hdrs = glob(["zkservice/include/*.h"]),
    srcs = ["zkservice/lib/libzkservice.a",],
    includes = ["zkservice/include",],
    visibility = ["//visibility:public",],
    deps = [
        ":zookeeper",
    ],

)

cc_library(
    name = "gtest",
    hdrs = glob(["gtest/include/**/*.h",
                "gtest/include/*.h"]),
    srcs = ["gtest/lib/libgtest.a",],
    includes = ["gtest/include",],
    visibility = ["//visibility:public",],
)

cc_library(
    name = "gbenchmark",
    hdrs = glob(["gbenchmark/include/**/*.h",
                "gbenchmark/include/*.h"]),
    srcs = ["gbenchmark/lib/libbenchmark.a",],
    includes = ["gbenchmark/include",],
    visibility = ["//visibility:public",],
)

cc_library(
    name = "leatherman",
    hdrs = glob([
        "leatherman/include/leatherman/**/*.hpp",
        "leatherman/include/leatherman/**/*.h",
    ]),
    srcs = glob(["leatherman/lib/*.a"]),
    includes = ["leatherman/include"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cpp_hocon",
    hdrs = glob([
        "cpp_hocon/include/**/*.hpp",
        "cpp_hocon/include/**/*.h"
    ]),
    srcs = glob(["cpp_hocon/lib/*.a"]),
    includes = ["cpp_hocon/include"],
    deps = ["//:leatherman",],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "muparserx",
    srcs = glob(["muparserx/muparserx/*.cpp"]),
    hdrs = glob(["muparserx/muparserx/*.h"]),
    includes = ["muparserx/"],
    copts = [
        "-std=c++11",
        "-Wall",
        "-pedantic",
        "-ffast-math",
        "-fomit-frame-pointer",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "tbb",
    hdrs = glob(["tbb/include/**/*.h",
                "tbb/include/*.h"]),
    srcs = ["tbb/lib/libtbb.so", "tbb/lib/libtbb.so.2",],
    includes = ["tbb/include",],
    visibility = ["//visibility:public",],
)

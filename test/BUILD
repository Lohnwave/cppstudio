COPTS = [
    "-DHAVE_PTHREAD",
    "-DHAVE_ZLIB",
    "-Wall",
    "-Wwrite-strings",
    "-Wdelete-non-virtual-dtor",
    "-Woverloaded-virtual",
    "-Wno-sign-compare",
    "-Wno-unused-function",
    "-Wno-ignored-qualifiers",
    "-Wno-float-equal",
    "-fopenmp",
    "-Ithirdparty/zlib-1.2.11",
    "-Wl,-Bsymbolic",
    "-ldl"
]
cc_test(
    name = 'json_read_test',
    srcs = 'json_read_test.cc',
    deps = [
        '//thirdparty/gtest:gtest',
        '//thirdparty/jsoncpp:jsoncpp',
        '//thirdparty/glog:glog',
    ],
    testdata = [
        './conf'
    ],
)

cc_binary(
    name = 'openmp',
    srcs = 'openmp.cc',
    deps = [
        '//thirdparty/glog:glog',
        '//thirdparty/jemalloc:jemalloc',
        '//thirdparty/muduo/base:muduo_base',
        '#dl',
        '#pthread',
        '#gomp'
    ],
    #optimize=['O3'],
    extra_cppflags = COPTS,
    extra_linkflags = [
        '-Wl,-rpath=/usr/local/lib',
        # 'Wl,-rpath=./build64_release/thirdparty/****'
    ]
)
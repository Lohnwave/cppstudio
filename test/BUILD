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
    ],
)
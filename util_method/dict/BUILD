cc_library(
    name = 'param_pool',
    srcs = 'param_pool.cc',
    deps = [
        '//thirdparty/muduo/base:muduo_base',
        '//thirdparty/jsoncpp:jsoncpp',
        '//toft/crypto/hash:md5',
    ]
)

cc_test(
    name = 'param_pool_test',
    srcs = 'param_pool_test.cc',
    deps = [
        ':param_pool',
        '//thirdparty/gtest:gtest',
        '//thirdparty/jsoncpp:jsoncpp',
        '//thirdparty/muduo/base:muduo_base'
    ],
    testdata = [
        './conf',
    ]
)
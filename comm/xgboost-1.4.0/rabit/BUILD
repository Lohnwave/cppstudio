cc_library(
    name = 'rabit',
    srcs = [
        "./src/allreduce_base.cc",
        "./src/allreduce_robust.cc",
        "./src/c_api.cc",
        "./src/engine.cc",
    ],
    deps = [
        '//thirdparty/muduo/base:muduo_base',
    ],
    extra_cppflags = [
        "-Icommon/xgboost-1.4.0/include/",
        "-Icommon/xgboost-1.4.0/dmlc-core/include/",
        "-Icommon/xgboost-1.4.0/rabit/include/",
    ]
)
cc_library(
    name = 'dmlc-core',
    srcs = [
        "./src/build_config.cc",
        "./src/config.cc",
        "./src/data.cc",
        "./src/io/filesys.cc",
        "./src/io/indexed_recordio_split.cc",
        "./src/io/input_split_base.cc",
        "./src/io/line_split.cc",
        "./src/io/local_filesys.cc",
        "./src/io/recordio_split.cc",
        "./src/io.cc",
        "./src/recordio.cc",
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
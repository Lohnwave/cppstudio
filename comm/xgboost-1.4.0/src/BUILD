cc_library(
    name = 'xgboost',
    srcs = [
        "./common/common.cc",
        "./common/hist_util.cc",
        "./common/host_device_vector.cc",
        "./c_api/c_api.cc",
        "./c_api/c_api_error.cc",
        "./data/data.cc",
        "./data/simple_csr_source.cc",
        "./data/simple_dmatrix.cc",
        "./data/sparse_page_dmatrix.cc",
        "./data/sparse_page_raw_format.cc",
        "./data/sparse_page_source.cc",
        "./data/sparse_page_writer.cc",
        "./gbm/gblinear.cc",
        "./gbm/gbm.cc",
        "./gbm/gbtree.cc",
        "./learner.cc",
        "./linear/linear_updater.cc",
        "./linear/updater_coordinate.cc",
        "./linear/updater_shotgun",
        "./logging.cc",
        "./metric/elementwise_metric.cc",
        "./metric/metric.cc",
        "./metric/multiclass_metric.cc",
        "./metric/rank_metric.cc",
        "./objective/hinge.cc",
        "./objective/multiclass_obj.cc",
        "./objective/objective.cc",
        "./objective/rank_obj.cc",
        "./objective/regression_obj.cc",
        "./predictor/cpu_predictor.cc",
        "./predictor/predictor.cc",
        "./tree/split_evaluator.cc",
        "./tree/tree_model.cc",
        "./tree/tree_updater.cc",
        "./tree/updater_colmaker.cc",
        "./tree/updater_histmaker.cc",
        "./tree/updater_prune.cc",
        "./tree/updater_quantile_hist.cc",
        "./tree/updater_refresh.cc",
        "./tree/updater_skmaker.cc",
        "./tree/updater_sync.cc",
    ],
    deps = [
        '//common/xgboost-1.4.0/dmlc-core:dmlc-core',
        '//common/xgboost-1.4.0/rabit:rabit',
        '//thirdparty/muduo/base:muduo_base',
    ],
    extra_cppflags = [
        "-Icommon/xgboost-1.4.0/include/",
        "-Icommon/xgboost-1.4.0/dmlc-core/include/",
        "-Icommon/xgboost-1.4.0/rabit/include/",
        "-Wno-non-virtual-dtor",
        "-Wno-overloaded-virtual",
    ]
)
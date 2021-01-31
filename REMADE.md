## issue#1 `glog: Fix Apple clang & libc++`
当在mac中使用thirdparty时`/src/glog/stl_logging.h:77:11: fatal error: 'ext/slist' file not found`

fix method see in [https://github.com/Dwight7walker4/google-glog/issues/121]
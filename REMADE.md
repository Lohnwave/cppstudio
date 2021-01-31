## issue#1 `glog: Fix Apple clang & libc++`
当在mac中使用thirdparty时`/src/glog/stl_logging.h:77:11: fatal error: 'ext/slist' file not found`

fix method see in [https://github.com/Dwight7walker4/google-glog/issues/121]

## issue#2 'Scons: except IOError, ex_value:'
似乎是高版本的scons采用的python版本不支持一些低版本语法。安装python2-scons
```
# python2-scons
wget http://prdownloads.sourceforge.net/scons/scons-3.1.2.tar.gz
cd scons-3.1.2
python bootstrap.py build/scons
python build/scons/setup.py install
```
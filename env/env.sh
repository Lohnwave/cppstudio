# blade install
cd ../thirdparty/typhoon-blade/./install
# gflags install
cd ../thirdparty/gflags-2.0/./configure && make && make install
# glog install gflags 2.0 : glog 0.3.4
cd ../thirdparty/glog-0.3.4/./configure && make && make install
# jemelloc install
cd ../thirdparty/jemelloc-5.2.1/./autogen.sh && ./configure && make && make install
# protobuf install
cd ../thirdparty/protobuf-2.4.1/./autogen.sh && ./configure && make && make install
# boost 1.75.0 install
cd ../thirdparty/boost_1_75_0/./bootstrap.sh && ./b2
# muduo 
# install ERROR fix boost not find: https://blog.csdn.net/ccf19881030/article/details/107850477
cd ../thirdparty/muduo-2.0.2/./build.sh
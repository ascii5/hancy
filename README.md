## 什么是hancy
hancy 是一个轻量的高性能服务器
## 启动
需要cmake工具，mysqlclientdev工具，mysql
进入项目路径
> sudo sh run.sh
## 目录树
```cpp
.
├── bin
│   └── server
├── build
│   ├── CMakeCache.txt
│   ├── CMakeFiles
│   ├── cmake_install.cmake
│   ├── Makefile
│   └── src
├── CMakeLists.txt
├── lib
│   └── libserverlib.a
├── LICENSE
├── log
│   ├── 2023_08_31_ServerLog
│   ├── 2023_09_03_ServerLog
│   ├── 2023_09_04_ServerLog
│   └── 2023_09_05_ServerLog
├── README.md
├── run.sh
├── src
│   ├── CGImysql
│   ├── CMakeLists.txt
│   ├── config
│   ├── http
│   ├── lock
│   ├── log
│   ├── main.cpp
│   ├── test_pressure
│   ├── threadpool
│   ├── timer
│   └── webserver
└── website
```
## 支持
支持config文件指定网站根目录
## todo
使用cpp11重构去除对pthrad库的依赖
## 关于
因为作者水平拙劣，有问题欢迎大家提出
该项目基础部分模仿自tinywebserver,它会更新新的功能  


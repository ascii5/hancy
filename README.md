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
reactor/proactor + 线程池 + 数据库连接池 + LT/ET读取数据 + 异步日志系统 + 定时器 + 状态机解析http报文  
文件上传前端使用form表格file控件发送post请求文件内容包含在body中，后端解析并存储相关文件并通过权限判断来返回是否正确存储  
每个用户可以上传文件且下载自己的或者公共的文件
## todo
- [x] cmake构建工程
- [x] 使用更为现代的cpp风格重构代码   
- [x] 更为规范的网站目录   
- [x] 文件上传服务功能更新
- [x] 文件下载功能更新
- [x] 时间堆定时器更新
- [x] 使用正则表达式进行数据解析
- [ ] 自动增长的缓冲区更新
- [x] 日志等级控制
- [ ] 单元测试更新
## 关于
因为作者水平拙劣，有问题欢迎大家提出
该项目基础部分模仿自tinywebserver,它会更新新的功能  


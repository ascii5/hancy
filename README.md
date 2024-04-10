## hancy
#### 什么是hancy
hancy 是一个基于Linux平台下使用`proactor`模型开发的轻量级的http服务器,支持静态网页，多媒体等资源的请求响应，文件的上传与下载
#### 环境依赖
- CMake(3,22,1)
- MySQL(8.0.36)
- ubuntu(22.04)
#### 项目启动
- 克隆仓库  
`git clone https://github.com/ascii5/hancy.git`  
- 进入项目路径  
`cd hancy`
- 编辑config文件
- 执行脚本      
`sudo sh run.sh`
#### 
#### 更新
- [x] cmake构建工程
- [x] 使用更为现代的cpp风格重构代码   
- [x] 更为规范的网站目录   
- [x] 文件上传服务功能更新
- [x] 文件下载功能更新
- [x] 时间堆定时器更新
- [x] 使用正则表达式进行数据解析
- [x] 日志等级控制
- [ ] 自动增长的缓冲区


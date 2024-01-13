#include <iostream>
#include <string>

namespace myDebug{
    void myDebug(std::string& message){
        std::cout<<message<<std::endl;
    }
}




//
// <!DOCTYPE html>
// <html>
// <head>
//     <title>文件下载示例</title>
// </head>
// <body>
//     <button onclick="downloadFile()">下载文件</button>

//     <script>
//         function downloadFile() {
//             // 发送下载请求
//             window.location.href = '/download';
//         }
//     </script>
// </body>
// </html>
// #include <iostream>
// #include <curl/curl.h>

// // 文件下载回调函数
// size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
//     size_t totalSize = size * nmemb;
//     buffer->append((char*)contents, totalSize);
//     return totalSize;
// }

// // 处理文件下载请求
// void handleDownloadRequest() {
//     CURL* curl = curl_easy_init();
//     if (curl) {
//         // 设置下载的文件URL
//         std::string fileUrl = "http://example.com/file.pdf";

//         // 设置保存文件的路径
//         std::string savePath = "/path/to/save/file.pdf";

//         // 设置文件下载回调函数
//         std::string response;
//         curl_easy_setopt(curl, CURLOPT_URL, fileUrl.c_str());
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

//         // 执行文件下载请求
//         CURLcode res = curl_easy_perform(curl);
//         if (res == CURLE_OK) {
//             // 保存下载的文件
//             FILE* file = fopen(savePath.c_str(), "wb");
//             if (file) {
//                 fwrite(response.c_str(), 1, response.size(), file);
//                 fclose(file);
//                 std::cout << "文件下载成功！" << std::endl;
//             } else {
//                 std::cout << "保存文件失败！" << std::endl;
//             }
//         } else {
//             std::cout << "文件下载失败！" << std::endl;
//         }

//         // 清理资源
//         curl_easy_cleanup(curl);
//     }
// }

// int main() {
//     // 处理文件下载请求
//     handleDownloadRequest();

//     return 0;
// }
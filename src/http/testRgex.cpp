#include<iostream>
#include<regex>

void testRegex(std::string requestLine){
    
    std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch submatch;
    if(regex_match(requestLine,submatch,patten)){
        std::string stringMethod = submatch[1];
        std::string stringUrl = submatch[2];
        std::string stringVersion = submatch[3];

        std::cout<<stringMethod<<std::endl;
        std::cout<<stringUrl<<std::endl;
        std::cout<<stringVersion<<std::endl;
    }
}
int main(){
    std::string requestLine("GET / HTTP/1.1");
    testRegex(requestLine);
    return 0;
}
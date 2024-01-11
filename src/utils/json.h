#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "myDebug.hpp"

class JSON{
public:
    JSON(){};
    JSON(const std::string& data);
    void parse(const std::string& data);
    const std::string& toJson();
    void set(std::string key,std::string value);
    void set(std::string key,std::vector<std::string> values);
    void set(std::string key,std::map<std::string,std::string> values);
    void get(const std::string& key,std::string& value);
    void get(const std::string& key,std::vector<std::string>& values);
    void get(const std::string& key,std::map<std::string,std::string>& values);
private:
    void parseString(std::stringstream& ss,std::string& str);
    void parseArry(std::stringstream& ss,std::vector<std::string>& mapValue);
    void parseObject(std::stringstream& ss);
    void parseObject(std::stringstream& ss,std::map<std::string,std::string>& valueMap);
    void appendMap(std::string& res);
    void appendMapVect(std::string& res);
    void appendMapMap(std::string& res);
private:
    std::map<std::string,std::string> mMap;
    std::map<std::string,std::vector<std::string>> mMapVector;
    std::map<std::string,std::map<std::string,std::string>> mMapMap;
};
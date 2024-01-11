#include "json.h"

JSON::JSON(const std::string& data){
    parse(data);
}

void JSON::parse(const std::string& data){
    std::stringstream ss(data);
    parseObject(ss);
}

void JSON::parseObject(std::stringstream& ss){
    char ch;
    std::string key,value;
    std::vector<std::string> values;
    std::map<std::string,std::string> valueMap;
    int judge = -1;
    while(ss >> ch){
        if(ch == '}')
            break;
        else if(ch == '\"'){
            parseString(ss,key);
            ss >> ch;
            ss >> ch;
            if(ch == '{'){
                judge = 0;
                parseObject(ss,valueMap);
            }
            else if(ch == '\"'){
                judge = 1;
                parseString(ss,value);
            }
            else if(ch == '['){
                judge = 2;
                parseArry(ss,values);
            }
        }
    }
    if(judge == -1)
        exit(-1);
    else if(judge == 0){
        mMapMap[key] = valueMap;
    }
    else if(judge == 1){
        mMap[key] = value;
    }
    else if(judge == 2){
        mMapVector[key] = values; 
    }
}

//支支持二重嵌套
void JSON::parseObject(std::stringstream& ss,std::map<std::string,std::string>& valueMap){
    char ch;

    std::string key,value;
    while(ss >> ch){
        if(ch == '}')
            break;
        else if(ch == '\"'){
            parseString(ss,key);
            ss >> ch;
            ss >> ch;
            parseString(ss,value);
            valueMap[key] = valueMap[value];
        }
        else if(ch == '[' || ch == '{'){
            exit(-1);
        }
    }

}

void JSON::parseString(std::stringstream& ss,std::string& str){
    std::getline(ss,str,'\"');
}
const std::string& JSON::toJson(){
    std::string res;
    res.push_back('{');
    appendMapVect(res);
    res.append(",");
    appendMap(res);
    res.append(",");
    appendMapMap(res);
    res.append("}");
    
}

void JSON::appendMap(std::string& res){
    for(auto it = mMap.begin();it != mMap.end();++it){
        res.push_back('\"');
        res.append(it -> first);
        res.push_back('\"');
        res.push_back(':');
        res.push_back('\"');
        res.append(it -> second);
        res.push_back('\"');
        res.push_back(',');
    }
    res.erase(res.end());
}

void JSON::appendMapVect(std::string& res){
    res.append("\"");
    auto it = mMapVector.begin();
    res.append(it -> first);
    res.append("\"");
    res.append(":");
    res.append("[");
    for(int i = 0;i < it -> second.size();++i){
        res.append("\"");
        res.append(it -> second[i]);
        res.append("\"");
        res.append(",");
    }
    res.erase(res.end());
    res.append("]");
    myDebug::myDebug(res);
}

void JSON::appendMapMap(std::string& res){
    auto it = mMapMap.begin();
    res.append("\"");
    res.append(it -> first);
    res.append("\"");
    res.append(":");
    
}
void JSON::parseArry(std::stringstream& ss,std::vector<std::string>& values){
    char ch;
    
    while(ss >> ch){
        if(ch == ']')
            break;
        else if(ch == '\"'){
            std::string str;
            parseString(ss,str);
            values.push_back(str);
        }
    }
}

void JSON::set(std::string key,std::string value){
    mMap[key] = value;
}

void JSON::set(std::string key,std::vector<std::string> values){
    mMapVector[key] = values;
}

void JSON::set(std::string key,std::map<std::string,std::string> values){
    mMapMap[key] = values;
}

void JSON::get(const std::string& key,std::string& value){
    value = mMap[key];
}

void JSON::get(const std::string& key,std::vector<std::string>& values){
    values = mMapVector[key];
}

void JSON::get(const std::string& key,std::map<std::string,std::string>& values){
    values = mMapMap[key];
}
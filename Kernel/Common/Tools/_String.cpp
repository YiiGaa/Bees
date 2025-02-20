#include "Kernel/Common/Tools/Tools.hpp"

//TIPS::Split the string according to the symbol into an array，such as: "aa,bb,cc" convert to ["a","bb","cc"]
//>>str::Target string
//>>pattern::Splitting Symbol
//>>return::vector<std::string>, splitting results
std::vector<std::string> Common::Tools::StringSplit(const std::string &str, const std::string &pattern){
    std::string::size_type pos;
    std::vector<std::string> result;
    std::string target = str+pattern;
    int size=target.size();

    for(int i=0; i<size; i++){
        pos=target.find(pattern,i);
        if(pos<size){
            std::string s=target.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

//TIPS::Replace string
//>>target::Target string
//>>key::Target key
//>>replacement::Replacement string
//>>isReplaceOne::Whether replace one
//>>return::true/false
bool Common::Tools::StringReplace(std::string &target, const std::string &key, const std::string &replacement, int isReplaceOne = 0){
    bool isReplace = false;
    size_t pos = 0;
    while ((pos = target.find(key, pos)) != std::string::npos) {
        target.replace(pos, key.length(), replacement);
        pos += replacement.length();
        isReplace = true;
        if(isReplaceOne){
            break;
        }
    }
    return isReplace;
}

//TIPS::Determine whether a string begins with a certain string
//>>target::Target string
//>>key::Prefix string
//>>return::true/false
bool Common::Tools::StringStartWith(std::string_view target, const std::string &key) {
    return target.substr(0, key.size()) == key;
}
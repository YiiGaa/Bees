#include "Kernel/Module/_DataCheck/_DataCheck.hpp"
#include "Kernel/Common/Common.hpp"
#include <iostream>
#include <string>

/* The same function as "Kernel/Module/Module.hpp" */
std::map<std::string, Module::Link*> moduleMap = {
    {"_DataCheck", new Module::_DataCheck()},
};
ERROR_CODE START_MODULE(std::string module, Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_LocateAdd("M-"+module);
    ERROR_CODE ret = (moduleMap[module])->Start(moduleParam, passParam);
    LOGGER_LocateDelete();
    return ret;
}
/* The same function as "Kernel/Module/Module.hpp" */

int main(int argc, char *argv[]){
    TOOLS_EndCrashListen(moduleMap);
    Json::Value passParam = Json::objectValue;
    Json::Value moduleParam = Json::objectValue;
    ERROR_CODE ret = ERR_OK;
    Json::Reader reader;

    //STEP::Init log
    LOGGER_InitSample();

    //STEP::Make passParam for test
    reader.parse(R"(
        {
            "key-1":1092349809823,
            "key-2":[123],
            "key-3":"teest",
            "key-4":"best",
            "key-5":13.34,
            "key-6":{
                "key-6-1":true,
                "key-6-2":["123", "456"],
                "key-6-3":[
                    {"key-6-3-1":"value","key-6-3-2":"value","key-6-3-3":123},
                    {"key-6-3-1":"value","key-6-3-2":"value","key-6-3-3":123},
                    true
                ],
                "key-6-4":{
                    "abc":{
                        "x":"123"
                    },
                    "aaa":{
                        "xx":[
                            "true",
                            {"xx":"yy"}
                        ]
                    }
                }
            },
            "key-7":{"key-7-1":"value","key-7-2":"value","key-7-3":123},
            "key-8":{"clean":true},
            "key-9":4.4
        }
    )", passParam);

    //STEP::Call module
    reader.parse(R"(
        {
            "_isClean":true,
            "_param":{
                "key-1":"long##",
                "key-2":"",
                "key-3":"reg##te+st",
                "key-4":"str##",
                "key-5":"double##13.340000/5.400000",
                "key-6":{
                    "key-6-1":true,
                    "key-6-2":[],
                    "key-6-3":[
                        {"key-6-3-1":"value","key-6-3-2":"value"},
                        true,
                        123
                    ],
                    "key-6-4>>abc>>x":"123",
                    "key-6-4>>aaa>>xx>>1>>xx":"yy"
                },
                "key-7":{},
                "key-9":4.4
            }
        }
    )", moduleParam);
    ret = START_MODULE("_DataCheck", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

end:
    //STEP::Print result
    std::cout<<std::endl<<"errorCode:"<<ERRORCODE_GetCode(ret)<<", message:"<<ERRORCODE_GetMessage(ret)<<std::endl;
    std::cout<<"passParam:"<<passParam.toStyledString()<<std::endl;

    //STEP::Clean module and close log
    TOOLS_EndClean();

    return ERRORCODE_IsError(ret)?-1:0;
}
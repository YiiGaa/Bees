#include "Kernel/Module/_DataFilling/_DataFilling.hpp"
#include "Kernel/Common/Common.hpp"
#include <iostream>
#include <string>

/* The same function as "Kernel/Module/Module.hpp" */
std::map<std::string, Module::Link*> moduleMap = {
    {"_DataFilling", new Module::_DataFilling()},
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
            "key-1":"value",
            "key-2":1,
            "key-3":true,
            "key-4":{
                "key-4-1":true,
                "key-4-2":[
                    {"key-4-2-2":"2"},
                    true,
                    123
                ]
            },
            "key-5":[
                {"key-5-1":"1", "key-5-2":"2"}
            ],
            "key-6":{
                "key-6-1":[
                    "1",
                    {"key-6-1-1":"123"}
                ]
            },
            "key-11": {
                "aaa":"aaa"
            }
        }
    )", passParam);

    //STEP::Call module
    reader.parse(R"(
        {
            "_action":"save",
            "_setting":{
                "key_1":"get##key-1"
            }
        }
    )", moduleParam);
    ret = START_MODULE("_DataFilling", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

    //STEP::Call module
    reader.parse(R"(
        {
            "_action":"set",
            "_setting":{
                "key-1":null,
                "key-2":11,
                "opt##key-3":3.4,
                "key-4":{
                    "key-4-1":3.5,
                    "key-4-2":[
                        {"key-4-2-1":"value"},
                        false
                    ]
                },
                "push##key-5":[
                    "123",
                    true
                ],
                "key-6>>key-6-1>>1>>key-6-1-1":false,
                "key-7":"uuid",
                "key-8":"random id",
                "key-10":"get##key-4",
                "key-12@get##key-2@":"time",
                "switch##key-11>>aaa":{
                    "reg##a":true
                },
                "key-15":"data##key_1"
            }
        }
    )", moduleParam);
    ret = START_MODULE("_DataFilling", moduleParam, passParam);
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
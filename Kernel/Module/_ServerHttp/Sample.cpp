#include "Kernel/Module/_ServerHttp/_ServerHttp.hpp"
#include "Kernel/Common/Common.hpp"
#include <iostream>
#include <string>
#include <curl/curl.h>

/* The same function as "Kernel/Module/Module.hpp" */
std::map<std::string, Module::Link*> moduleMap = {
    {"_ServerHttp", new Module::_ServerHttp()},
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
        }
    )", passParam);

    //STEP::Call module
    reader.parse(R"(
        {
            "_url":"https://localhost:3000/test?a=1#sdf",
            "_method":"POST",
            "_header":{
                "key":"value"
            },
            "_param":{
                "key-1":"value-1"
            },
            "_resultKey":"$result"
        }
    )", moduleParam);
    ret = START_MODULE("_ServerHttp", moduleParam, passParam);
    TOOLS_JsonClean(moduleParam);

    std::cout<<std::endl<<passParam.toStyledString()<<std::endl;
    std::cout<<"errorCode:"<<ERRORCODE_GetCode(ret)<<",message:"<<ERRORCODE_GetMessage(ret)<<std::endl;

    return ERRORCODE_IsError(ret)?-1:0;
}
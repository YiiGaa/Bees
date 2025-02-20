#include "Kernel/Module/_DataQueue/_DataQueue.hpp"
#include "Kernel/Common/Common.hpp"
#include <iostream>
#include <string>

/* The same function as "Kernel/Module/Module.hpp" */
std::map<std::string, Module::Link*> moduleMap = {
    {"_DataQueue", new Module::_DataQueue()},
};
ERROR_CODE START_MODULE(std::string module, Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_LocateAdd("M-"+module);
    ERROR_CODE ret = (moduleMap[module])->Start(moduleParam, passParam);
    LOGGER_LocateDelete();
    return ret;
}
/* The same function as "Kernel/Module/Module.hpp" */

void TestThread(){
    Json::Value passParam = Json::objectValue;
    Json::Value moduleParam = Json::objectValue;
    ERROR_CODE ret = ERR_OK;
    Json::Reader reader;

    //STEP::Call module
    reader.parse(R"(
        {
            "_action":"pull",
            "_identity":"mission",
            "_resultKey":"$result",
            "_dataIdKey":"$dataId",
            "_isPop":false,
            "_isWait":true,
        }
    )", moduleParam);
    ret = START_MODULE("_DataQueue", moduleParam, passParam);
    TOOLS_JsonClean(moduleParam);

    //STEP::For testing
    int counter = 10;
    while(counter--){
        LOGGER_Console("**TestThread count:"+std::to_string(counter)+",passParam:"+passParam.toStyledString());
        sleep(1);
    }

    //STEP::Call module
    reader.parse(R"(
        {
            "_action":"pop",
            "_identity":"mission",
            "_id":"get##$dataId"
        }
    )", moduleParam);
    ret = START_MODULE("_DataQueue", moduleParam, passParam);
    TOOLS_JsonClean(moduleParam);

    //STEP::Print result
    std::cout<<std::endl<<"**TestThread errorCode:"<<ERRORCODE_GetCode(ret)<<", message:"<<ERRORCODE_GetMessage(ret)<<std::endl;
    std::cout<<"passParam:"<<passParam.toStyledString()<<std::endl;
}

ERROR_CODE TestCallBack(Json::Value &passParam){
    Json::Value moduleParam = Json::objectValue;

    LOGGER_Console("**TestCallBack timout callback");

    return ERR_OK;
}

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
            "mission":{
                "key":"value"
            }
        }
    )", passParam);

    //STEP::Mark function for test
    TOOLS_CallFunctionMark("TestCallBack",TestCallBack);

    //STEP::Call module
    reader.parse(R"(
        {
            "_action":"create",
            "_identity":"mission",
            "_queueMaxLen":0
        }
    )", moduleParam);
    ret = START_MODULE("_DataQueue", moduleParam, passParam);
    TOOLS_JsonClean(moduleParam);

    //STEP::Create thread for test, for pulling data
    std::thread threadHandler(TestThread);

    //STEP::Call module
    reader.parse(R"(
        {
            "_action":"push",
            "_identity":"mission",
            "_data":"get##mission",
            "_timeout":5,
            "_timeoutCall":"TestCallBack",
            "_isFullWait":true
        }
    )", moduleParam);
    ret = START_MODULE("_DataQueue", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

    //STEP::Call module
    reader.parse(R"(
        {
            "_action":"delete",
            "_identity":"mission"
        }
    )", moduleParam);
    ret = START_MODULE("_DataQueue", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

end:
    //STEP::Print result
    std::cout<<std::endl<<"errorCode:"<<ERRORCODE_GetCode(ret)<<", message:"<<ERRORCODE_GetMessage(ret)<<std::endl;
    std::cout<<"passParam:"<<passParam.toStyledString()<<std::endl;

    //STEP::Wait test thread end
    threadHandler.join();

    //STEP::Clean module and close log
    TOOLS_EndClean();

    return ERRORCODE_IsError(ret)?-1:0;
}
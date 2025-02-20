#include "Kernel/Common/Common.hpp"
#include "Kernel/Module/Module.hpp"
#include "Kernel/Task/Task.hpp"
#include "main.hpp"
#include <iostream>
#include <string>
#include <json/json.h>
#include <regex>
#include <stdio.h>

int main(int argc, char* argv[]){
    Json::Value moduleParam;
    Json::Value passParam;
    Json::FastWriter fastWriter;
    fastWriter.omitEndingLineFeed();

    //STEP::For fixed passParam(Usually for testing)
    std::string fixedParam = Main::fixedParam;

    //STEP::Check param
    if(argc < 2){
        std::cout<< "● version:" << CONFIG_All_Version <<std::endl;
        std::cout<<"※ Please enter param like: ./Boot \"{}|xx.json|version|fixed\" \"process name\"(option)"<<std::endl;
        return -2;
    }

    //STEP::Get passParam
    std::string param = argv[1];
    std::regex pattern("(.+?)\\.json$");
    //Mode 1::print version
    if(param == "version"){
        std::cout<< "● version:" << CONFIG_All_Version <<std::endl;
        return 0;
    }
    //Mode 2::Read json file as param
    else if(std::regex_search(param, pattern)){
        std::cout<<"● Boot mode: File mode"<<std::endl;
        if(TOOLS_JsonRead(param, passParam)){
            std::cout<<"※ Read file failed: "<<param<<std::endl;
            return -3;
        }
    }
    //Mode 3::Fixed passParam
    else if(param == "fixed"){
        std::cout<<"● Boot mode: fixed mode"<<std::endl;
        Json::Reader reader;
        if(!reader.parse(fixedParam, passParam)){
            std::cout<<"※ Trans json failed: "<<param<<std::endl;
            return -4;
        }
    }
    //Mode 4::Json string as param
    else {
        Json::Reader reader;
        std::cout<<"● Boot mode: command line"<<std::endl;
        if(!reader.parse(param, passParam)){
            std::cout<<"※ Deal json failed: "<<param<<std::endl;
            return -5;
        }
    }

    //STEP::Get necessary param: identity、logRoot
    std::string _identity = TOOLS_ParamGet("identity", "BeesBoot", passParam);
    std::string _logRoot = TOOLS_ParamGet("logRoot", CONFIG_All_Logger_Root, passParam);
    LOGGER_Init(_logRoot, _identity);

    //STEP::Set process name
    if(argc >= 3){
        int size = 0;
        for (int i = 0; i< argc; i++) {
            size += strlen(argv[i]) + 1;
        }
        strcpy(argv[0], argv[2]);
        memset(argv[0]+strlen(argv[0])+1, 0, size-(strlen(argv[2])+1));
    }

    //STEP::Init Task Thread/Function Mark
    TOOLS_CallFunctionMark(Task::taskFunctionMap);
    TOOLS_CallThreadMark(Task::taskMap);

    //STEP::Mark Crash moduleMap
    TOOLS_EndCrashListen(Module::moduleMap);

    //STEP::Run Task
    ERROR_CODE ret = ERR_OK;
    LOGGER_Info("● Boot start task(Start)", moduleParam, passParam);
    if(TOOLS_CallThreadCheck("Start")){
        LOGGER_LocateDelete();
        LOGGER_LocateDelete();
        ret = TOOLS_CallThread("Start", passParam);
    } else {
        std::cout<<"※ Missing \"Start\" Task"<<std::endl;
        LOGGER_Error("※ Missing \"Start\" Task");
        TOOLS_EndClean();
        return -6;
    }

    //STEP::Clean module and close log
    LOGGER_LocateAdd("Bees Bootstrap");
    LOGGER_LocateAdd(__FUNCTION__);
    LOGGER_Info("● Boot end, errorCode:"+ERRORCODE_GetCode(ret)+", message:"+ERRORCODE_GetMessage(ret), moduleParam, passParam);
    TOOLS_EndClean();

    return ERRORCODE_IsError(ret)?-1:0;
}
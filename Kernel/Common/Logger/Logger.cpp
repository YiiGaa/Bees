#include "Kernel/Common/Logger/Logger.hpp"
#include "Kernel/Common/Config/Config.hpp"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <fmt/format.h>
#include <iostream>
#include <thread>
#include <string>

std::shared_ptr<spdlog::logger> Common::Logger::loggerHandler = NULL;
std::shared_ptr<spdlog::logger> Common::Logger::loggerConsoleHandler = NULL;
Json::FastWriter Common::Logger::loggerWriter;
std::map<std::string ,std::vector<std::string>> Common::Logger::threadIdMap;

void Common::Logger::LocateAdd(std::string name){
    std::thread::id threadId = std::this_thread::get_id();
    std::ostringstream oss;
    oss << threadId;
    std::string threadIdStr = oss.str();
    if(threadIdMap.count(threadIdStr)){
        threadIdMap[threadIdStr].push_back(name);
    } else {
        std::vector<std::string> callVector;
        callVector.push_back(name);
        threadIdMap[threadIdStr] = callVector;
    }
    return;
}

void Common::Logger::LocateDelete(){
    std::thread::id threadId = std::this_thread::get_id();
    std::ostringstream oss;
    oss << threadId;
    std::string threadIdStr = oss.str();
    if(threadIdMap.count(threadIdStr)){
        if(threadIdMap[threadIdStr].size()>1){
            threadIdMap[threadIdStr].pop_back();
        } else {
            threadIdMap.erase(threadIdStr);
        }
    }
    return;
}

void Common::Logger::LocateGet(std::string &task, std::string &module){
    std::thread::id threadId = std::this_thread::get_id();
    std::ostringstream oss;
    oss << threadId;
    std::string threadIdStr = oss.str();
    if(threadIdMap.count(threadIdStr)){
        std::vector<int>::size_type size = threadIdMap[threadIdStr].size();
        if(size>1){
            task = threadIdMap[threadIdStr].front();
            module = threadIdMap[threadIdStr].back();
        } else if(size>0){
            task = threadIdMap[threadIdStr].front();
        }
    }else{
        task = "-";
        module = "-";
    }
    return;
}

void Common::Logger::InitLogger(std::string root, std::string id, int isDebug, std::string function){
    //STEP::Set json string without
    loggerWriter.omitEndingLineFeed();

    //STEP::Set Logger
    if(isDebug){
        if(Logger::loggerHandler!=NULL){
            return;
        }
        loggerHandler = spdlog::stdout_color_mt("logger");
        loggerHandler->set_level(spdlog::level::debug);
    } else {
        if(Logger::loggerHandler!=NULL){
            loggerHandler->flush();
        }
        std::string fileName = (root==""?CONFIG_All_Logger_Root:root) + id + ".log";
        loggerHandler = spdlog::rotating_logger_mt("logger", fileName, 1024 * 1024 * 10, 3);
        if(!loggerHandler){
            std::cout << "ERROR: mission log create failed!"<< std::endl;
        }
        loggerHandler->set_level(spdlog::level::info);
        loggerHandler->flush_on(spdlog::level::err);
        spdlog::flush_every(std::chrono::seconds(3));
    }
    loggerHandler->set_pattern(CONFIG_Logger_Pattern);

    //STEP::Set console log
    if(Logger::loggerConsoleHandler!=NULL){
        return;
    }
    loggerConsoleHandler = spdlog::stdout_color_mt("console");
    loggerConsoleHandler->set_level(spdlog::level::info);
    loggerConsoleHandler->flush_on(spdlog::level::info);
    loggerConsoleHandler->set_pattern("%v");

    //STEP::Init locate
    LOGGER_LocateAdd("Bees Bootstrap");
    LOGGER_LocateAdd(function);

    return;
}

void Common::Logger::PrintLogger(int level, std::string logger, const Json::Value& moduleParam, const Json::Value& passParam, std::string file, std::string function, int line){
    if(Logger::loggerHandler==NULL){
        return;
    }
    std::string task = "-";
    std::string module = "-";
    LocateGet(task, module);
    if(level == 0){
        loggerHandler->error(CONFIG_Logger_Format, task, module, logger, file, function, line, loggerWriter.write(moduleParam), loggerWriter.write(passParam));
    } else if(level == 1) {
        loggerHandler->info(CONFIG_Logger_Format, task, module, logger, file, function, line, loggerWriter.write(moduleParam), loggerWriter.write(passParam));
    } else if(level == 2) {
        loggerHandler->debug(CONFIG_Logger_Format, task, module, logger, file, function, line, loggerWriter.write(moduleParam), loggerWriter.write(passParam));
    }
    return;
}

void Common::Logger::PrintLogger(int level, std::string logger, std::string file, std::string function, int line){
    if(Logger::loggerHandler==NULL){
        return;
    }
    std::string task = "-";
    std::string module = "-";
    LocateGet(task, module);
    if(level == 0){
        loggerHandler->error(CONFIG_Logger_Format, task, module, logger, file, function, line, "{-}", "{-}");
    } else if(level == 1) {
        loggerHandler->info(CONFIG_Logger_Format, task, module, logger, file, function, line, "{-}", "{-}");
    } else if(level == 2) {
        loggerHandler->debug(CONFIG_Logger_Format, task, module, logger, file, function, line, "{-}", "{-}");
    }
    return;
}

void Common::Logger::PrintLogger(std::string logger){
    loggerConsoleHandler->info(logger);
    return;
}

void Common::Logger::CloseLogger(){
    if(Logger::loggerHandler!=NULL){
        loggerHandler->flush();
    }
    if(Logger::loggerConsoleHandler!=NULL){
        loggerConsoleHandler->flush();
    }
    spdlog::shutdown();
    return;
}


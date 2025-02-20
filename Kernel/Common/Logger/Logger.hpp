#ifndef   _COMMON_Logger_HPP_
#define   _COMMON_Logger_HPP_

#define CONFIG_Logger_Pattern "[%Y-%m-%d %H:%M:%S][%l][%t]%v"
#define CONFIG_Logger_Format "[{}][{}] {},{},{}(),{},moduleParam:{},passParam:{}"

#if CONFIG_All_DEBUG == true
    #define LOGGER_InitSample() Common::Logger::InitLogger("", "Sample", 1, __FUNCTION__)
    #define LOGGER_Init(logRoot, id) Common::Logger::InitLogger(logRoot, id, 1, __FUNCTION__)
    #define LOGGER_Debug(...) Common::Logger::PrintLogger(2, __VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)
#else
    #define LOGGER_InitSample() Common::Logger::InitLogger("", "Sample", 0, __FUNCTION__)
    #define LOGGER_Init(logRoot, id) Common::Logger::InitLogger(logRoot, id, 0, __FUNCTION__)
    #define LOGGER_Debug(...)
#endif

#define LOGGER_Close() Common::Logger::CloseLogger()
#define LOGGER_Error(...) Common::Logger::PrintLogger(0, __VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)
#define LOGGER_Info(...) Common::Logger::PrintLogger(1, __VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)
#define LOGGER_Console(log) Common::Logger::PrintLogger(log)

#define LOGGER_LocateAdd(name) Common::Logger::LocateAdd(name)
#define LOGGER_LocateDelete() Common::Logger::LocateDelete()

#include <iostream>
#include <json/json.h>
#include <spdlog/spdlog.h>
#include <map>
#include <vector>

namespace Common{
    class Logger {
        public:
            static void LocateAdd(std::string);
            static void LocateDelete();
            static void LocateGet(std::string&, std::string&);

            static void InitLogger(std::string, std::string, int, std::string);
            static void CloseLogger();
            static void PrintLogger(int, std::string, const Json::Value&, const Json::Value&, std::string, std::string, int);
            static void PrintLogger(int, std::string, std::string, std::string, int);
            static void PrintLogger(std::string);

        public:
            static std::map<std::string, std::vector<std::string>> threadIdMap;
            static std::shared_ptr<spdlog::logger> loggerHandler;
            static std::shared_ptr<spdlog::logger> loggerConsoleHandler;
            static Json::FastWriter loggerWriter;
    };
}

#endif
#include "Kernel/Module/_DataFilling/_DataFilling.hpp"
#include <string_view>
#include <unistd.h>
#include <time.h>
#include <regex>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdlib>

using namespace Module;

Json::Value _DataFilling::Filling_FindDataPool(std::vector<std::string> targetArr, Json::Value& data){
    //STEP::Init default result
    Json::Value result = "null";

    //WHEN::Return when find end
    if(targetArr.size() == 0 ||
       (targetArr.size() == 1 && targetArr[0]=="")){
        return data;
    }

    //STEP::Get find key
    std::string key = targetArr[0];
    targetArr.erase(targetArr.begin());

    //WHEN::Find deeper when data is object and data has this key
    if(data.type() == Json::ValueType::objectValue && data.isMember(key)){
        result = this->Filling_FindDataPool(targetArr, data[key]);
        return result;
    }

    //WHEN::Data is array
    if(data.type() == Json::ValueType::arrayValue){
        //STEP-IN-IN::Change key to index
        int index = -1;
        try {
            index = std::stoi(key);
        } catch (const std::invalid_argument& e) {index = -1;} catch (const std::out_of_range& e) {index = -1;}

        //STEP-IN-IN::Check index is invalid or not
        if(index<0){
            return result;
        }

        //STEP-IN-IN::Find deeper when data has index
        if(data.size()>index){
            result = this->Filling_FindDataPool(targetArr, data[index]);
            return result;
        }
    }

    return result;
}

std::string _DataFilling::Filling_GenerateRandomString(size_t length) {
    const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    boost::random::random_device rd;
    boost::random::mt19937 gen(rd());
    boost::random::uniform_int_distribution<> dist(0, chars.size() - 1);
    std::string result;
    for (size_t i = 0; i < length; ++i) {
        result += chars[dist(gen)];
    }
    return result;
}

Json::Value _DataFilling::Filling(std::string& target, Json::Value& passParam){
    std::string result = "";
    Json::FastWriter fastWriter;
    fastWriter.omitEndingLineFeed();

    //STEP::Split "+", and handle each piece
    std::vector<std::string> blockArr = TOOLS_StringSplit(target, "+");
    for(int i=0;i<blockArr.size();i++){
        //STEP::Deal ++ means +
        if(blockArr[i] == ""){
            result+= "+";
            continue;
        }

        //STEP::Split function and parameter
        std::vector<std::string> pieceArr = TOOLS_StringSplit(blockArr[i], "##");
        std::string_view fun(pieceArr[0]);

        //WHEN-IN::getPid mode
        if(fun == "getPid"){
            result+= std::to_string((int)getpid());
            continue;
        }

        //WHEN-IN::time mode, format like 2000-01-01 08:00:00
        if(fun == "time"){
            std::string format = "%Y-%m-%d %H:%M:%S";
            if(pieceArr.size()>1){
                format = pieceArr[1];
            }
            auto now = std::chrono::system_clock::now();
            std::time_t rawTime = std::chrono::system_clock::to_time_t(now);
            struct std::tm* timeinfo = localtime(&rawTime);
            std::stringstream ss;
            ss << std::put_time(timeinfo, format.c_str());
            result += ss.str();
            continue;
        }

        //WHEN-IN::timeStamp mode
        if(fun == "timeStamp"){
            time_t timestamp = time(NULL);
            result += std::to_string(timestamp);
            continue;
        }

        //WHEN-IN::uuid mode
        if(fun == "uuid"){
            boost::uuids::uuid uuid = boost::uuids::random_generator()();
            std::string uuidStr = boost::uuids::to_string(uuid);
            TOOLS_StringReplace(uuidStr, "-", "");
            result += uuidStr;
            continue;
        }

        //WHEN-IN::uuid mode
        if(fun == "random id"){
            size_t length = 8;
            if(pieceArr.size()>1){
                try {
                    length = std::stoull(pieceArr[1]);
                } catch (...) {}
            }
            result += Filling_GenerateRandomString(length);
            continue;
        }

        //WHEN-IN::get mode, get from passParam
        if(fun == "get"){
            if(pieceArr.size()<2){
                result += blockArr[i];
                continue;
            }

            //STEP-IN-IN::Find data
            std::vector<std::string> innerArr = TOOLS_StringSplit(pieceArr[1], ">>");
            Json::Value resultGet = this->Filling_FindDataPool(innerArr, passParam);

            //STEP-IN-IN::Add results
            if(i==0 && blockArr.size() == 1){
                return resultGet;
            } else if(resultGet.type() == Json::ValueType::stringValue){
                result += resultGet.asString();
            } else {
                result += fastWriter.write(resultGet);
            }

            continue;
        }

        //WHEN-IN::data mode, get data stored in save mode
        if(fun == "data"){
            if(pieceArr.size()<2){
                result += blockArr[i];
                continue;
            }

            //STEP-IN-IN::Read lock
            TOOLS_LockRead(dataPoolMutex, lock);

            //STEP-IN-IN::Find data
            std::vector<std::string> innerArr = TOOLS_StringSplit(pieceArr[1], ">>");
            Json::Value resultGet = this->Filling_FindDataPool(innerArr, dataPool);

            //STEP-IN-IN::Add results
            if(i==0 && blockArr.size() == 1){
                return resultGet;
            } else if(resultGet.type() == Json::ValueType::stringValue){
                result += resultGet.asString();
            } else {
                result += fastWriter.write(resultGet);
            }

            //STEP-IN-IN::Unlock
            lock.unlock();

            continue;
        }

        //WHEN-IN::no mode
        result += blockArr[i];
    }

    return result;
}
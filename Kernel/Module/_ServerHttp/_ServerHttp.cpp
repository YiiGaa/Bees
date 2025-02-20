#include "Kernel/Module/_ServerHttp/_ServerHttp.hpp"
#include <iostream>
#include <string>
#include <httplib.h>
#include <boost/url.hpp>

using namespace Module;

//TIPS::Constructor function
//Each module will only have 1 object (created at program startup), please fully consider the impact of multithreading
_ServerHttp::_ServerHttp(void){
}

std::string _ServerHttp::URLEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped << std::hex << std::setfill('0');
    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
        }
    }
    return escaped.str();
}

ERROR_CODE _ServerHttp::DoStart(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    try {
        //STEP::Get setting
        std::string _url = TOOLS_ParamRead("_url", "", moduleParam, passParam);
        std::string _method = TOOLS_ParamRead("_method", "POST", moduleParam, passParam);
        Json::Value _param = TOOLS_ParamRead("_param", Json::nullValue, moduleParam, passParam);
        std::string _paramType = TOOLS_ParamRead("_paramType", "application/json", moduleParam, passParam);
        Json::Value _header = TOOLS_ParamRead("_header", Json::objectValue, moduleParam, passParam);
        std::string _resultKey = TOOLS_ParamRead("_resultKey", "", moduleParam, passParam);
        bool _isVerifySSL = TOOLS_ParamRead("_isVerifySSL", false, moduleParam, passParam);
        LOGGER_Debug("Module-_ServerHttp DoStart _url:" + _url);
        LOGGER_Debug("Module-_ServerHttp DoStart _param:" + _param.toStyledString());
        LOGGER_Debug("Module-_ServerHttp DoStart _paramType:" + _paramType);
        LOGGER_Debug("Module-_ServerHttp DoStart _header:" + _header.toStyledString());
        LOGGER_Debug("Module-_ServerHttp DoStart _resultKey:" + _resultKey);
        LOGGER_Debug("Module-_ServerHttp DoStart _isVerifySSL:" + std::to_string(_isVerifySSL));

        //STEP::Convert url
        auto urlObject = boost::urls::parse_uri(_url);
        if (!urlObject) {
            LOGGER_Error("_ServerHttp Url illegal(convert fail), _url"+_url);
            return ERR_Module__ServerHttp_Url_Illegal;
        }
        _isVerifySSL = std::string(urlObject->scheme())=="https" && _isVerifySSL;
        std::string urlHost = std::string(urlObject->scheme())+"://"+std::string(urlObject->host())+":"+std::string(urlObject->port());
        std::string urlPath = std::string(urlObject->path());
        std::string urlQuery = std::string(urlObject->query());
        std::string urlFragment = std::string(urlObject->fragment());

        //WHEN::GET, Convert param to url
        Json::FastWriter jsonWriter;
        jsonWriter.omitEndingLineFeed();
        if(_method == "GET" && _param.type() == Json::objectValue) {
            std::string paramStr = "";
            for (Json::ValueIterator it = _param.begin(); it != _param.end(); ++it) {
                std::string key = this->URLEncode(it.key().asString());
                std::string value = this->URLEncode((*it).type()==Json::stringValue?(*it).asString():jsonWriter.write(*it));
                paramStr += key + "=" + value + "&";
            }
            if (paramStr != "") {
                paramStr.pop_back();
                urlQuery += (urlQuery==""?"":"&") + paramStr;
            }
        }

        //STEP::Create connection(not send yet)
        urlPath += (urlQuery==""?"":"?") + urlQuery + (urlFragment==""?"":"#") + urlFragment;
        LOGGER_Info("_ServerHttp Request host:"+urlHost);
        LOGGER_Info("_ServerHttp Request path:"+urlPath);
        httplib::Client client(urlHost);
        client.enable_server_certificate_verification(_isVerifySSL);
        client.set_connection_timeout(CONFIG__ServerHttp_TimeoutConnect,0);
        client.set_read_timeout(CONFIG__ServerHttp_TimeoutRead, 0);
        client.set_write_timeout(CONFIG__ServerHttp_TimeoutWrite, 0);

        //STEP::Add header
        httplib::Headers headers;
        std::string headersLog = "";
        for(Json::ValueIterator it = _header.begin(); it != _header.end(); ++it) {
            std::string value = (*it).type()==Json::stringValue?(*it).asString():jsonWriter.write(*it);
            headers.insert({it.key().asString(), value});
            headersLog += it.key().asString() + ":" + value+"~";
        }
        LOGGER_Info("_ServerHttp Request headers:"+headersLog);

        //STEP::Send request
        httplib::Result result;
        if(_method == "POST") {
            LOGGER_Info("_ServerHttp Request param:"+_param.toStyledString());
            result = client.Post(urlPath, headers ,jsonWriter.write(_param), _paramType);
        }else if(_method == "DELETE") {
            LOGGER_Info("_ServerHttp Request param:"+_param.toStyledString());
            result = client.Delete(urlPath, headers ,jsonWriter.write(_param), _paramType);
        }else if(_method == "PATCH") {
            LOGGER_Info("_ServerHttp Request param:"+_param.toStyledString());
            result = client.Patch(urlPath, headers ,jsonWriter.write(_param), _paramType);
        }else if(_method == "PUT") {
            LOGGER_Info("_ServerHttp Request param:"+_param.toStyledString());
            result = client.Put(urlPath, headers ,jsonWriter.write(_param), _paramType);
        }else if(_method == "GET") {
            result = client.Get(urlPath, headers);
        }else {
            LOGGER_Error("_ServerHttp Request method illegal, _method:"+_method);
            return ERR_Module__ServerHttp_Method_Illegal;
        }

        //WHEN::Error occur
        if(!result) {
            LOGGER_Error("_ServerHttp Connect fail");
            return ERR_Module__ServerHttp_Connect_Fail;
        }else if(result->status < 200||result->status >= 300) {
            LOGGER_Error("_ServerHttp Response fail, code:"+std::to_string(result->status));
            return ERR_Module__ServerHttp_Response_Fail;
        }

        //STEP-In::Insert result
        LOGGER_Info("_ServerHttp Response:"+std::string(result->body));
        Json::Value responseJson;
        Json::Reader reader;
        reader.parse(std::string(result->body), responseJson);
        if(_resultKey!="" || responseJson.type() != Json::ValueType::objectValue){
            passParam[_resultKey] = responseJson;
        }else{
            passParam = responseJson;
        }

    } catch (std::exception& e){
        LOGGER_Error(std::string(e.what()), moduleParam, passParam);
        return ERR_Module__ServerHttp_Exception;
    } catch (...){
        LOGGER_Error("unknown exception, maybe Segmentation fault (core dumped)", moduleParam, passParam);
        return ERR_Module__ServerHttp_Exception;
    }

    return ret;
}

//TIPS::Program end or crash will call this
//Please note the impact of multithreading
void _ServerHttp::Crash(int isClean = 0){
    try{
        //STEP::Print log
        if(isClean)
            LOGGER_Info("_ServerHttp Clean deal");
        else
            LOGGER_Info("_ServerHttp Crash deal");

        //STEP::Do Clean

    } catch (std::exception& e){
        LOGGER_Error("_ServerHttp Clean exception:"+std::string(e.what()));
    } catch (...){
        LOGGER_Error("_ServerHttp Clean unknown exception, maybe Segmentation fault (core dumped)");
    }
}

//TIPS::Module entrance
ERROR_CODE _ServerHttp::Start(Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_Debug("Module-_ServerHttp start", moduleParam, passParam);
    ERROR_CODE ret = ERR_OK;
    ret = this->DoStart(moduleParam, passParam, ret);
    LOGGER_Debug("Module-_ServerHttp end, errorCode:"+ERRORCODE_GetCode(ret)+", message:"+ERRORCODE_GetMessage(ret), moduleParam, passParam);
    return ret;
}
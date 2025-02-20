#ifndef   _MODULE__DataCheck_HPP_
#define   _MODULE__DataCheck_HPP_

#include "Kernel/Common/Common.hpp"
#include "Kernel/Module/Link.hpp"
#include <json/json.h>
#include <map>
#include <mutex>
#include <shared_mutex>

namespace Module{
    class _DataCheck: public Link{
        private:
            int CheckParam_Do(std::string, std::string, Json::Value&);
            ERROR_CODE CheckParam_TypeCheck(std::string&, std::string&, Json::Value&);
            ERROR_CODE CheckParam(std::string, Json::Value&, Json::Value&, Json::Value&);
            int Traverse_ListMaxLength(std::string&);
            Json::Value* Traverse_FindJsonValue(std::vector<std::string>, Json::Value&);
            Json::Value* Traverse_InitJsonPath(std::vector<std::string>, Json::Value&);
            ERROR_CODE Traverse(Json::Value&, Json::Value&, Json::Value&, Json::Value&);
            ERROR_CODE DoStart(Json::Value&, Json::Value&, ERROR_CODE);
        
        public:
            void Crash(int);
            _DataCheck();
            ERROR_CODE Start(Json::Value&, Json::Value&);
    };
}

#endif
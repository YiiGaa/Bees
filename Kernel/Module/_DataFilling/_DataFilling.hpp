#ifndef   _MODULE__DataFilling_HPP_
#define   _MODULE__DataFilling_HPP_

#include "Kernel/Common/Common.hpp"
#include "Kernel/Module/Link.hpp"
#include <json/json.h>
#include <map>
#include <vector>
#include <mutex>
#include <shared_mutex>

namespace Module{
    class _DataFilling: public Link{
        //TIPS::Variables in the module
        private:
            static Json::Value dataPool;
            static std::shared_mutex dataPoolMutex;

        private:
            bool Traverse_CustomKey(std::string&, Json::Value&);
            Json::Value* Traverse_FindJsonValue(std::vector<std::string>, Json::Value&, bool);
            Json::Value* Traverse_InitJsonPath(std::vector<std::string>, Json::Value&);
            ERROR_CODE Traverse(Json::Value&, Json::Value&, Json::Value&, bool);
            std::string Filling_GenerateRandomString(size_t);
            Json::Value Filling_FindDataPool(std::vector<std::string>, Json::Value&);
            Json::Value Filling(std::string&, Json::Value&);
            ERROR_CODE Switch(Json::Value&, Json::Value&, Json::Value&, bool);

        //TIPS::Action processing function
        private:
            ERROR_CODE Save(Json::Value&, Json::Value&, ERROR_CODE);
            ERROR_CODE Set(Json::Value&, Json::Value&, ERROR_CODE);
            ERROR_CODE DoStart(Json::Value&, Json::Value&, ERROR_CODE);
        
        public:
            void Crash(int);
            _DataFilling();
            ERROR_CODE Start(Json::Value&, Json::Value&);
    };
}

#endif
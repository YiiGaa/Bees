#ifndef   _MAIN_HPP_
#define   _MAIN_HPP_

namespace Main{
    //Boot以"fixed"模式启动时，用fixedParam填充passParam
    //fixedParam内容可手动修改，框架更新时，此文件不会强制更新
    //When Boot starts in "fixed" mode, fill passParam with fixedParam
    //The content of fixedParam can be modified manually. When the framework is updated, this file will not be forcibly updated
    std::string fixedParam = R"(
        {
        }
    )";
}

#endif
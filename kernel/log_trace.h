//
// Created by wenzel on 05.01.22.
//

#ifndef LOG_TRACE_H
#define LOG_TRACE_H

#include "yosys.h"

YOSYS_NAMESPACE_BEGIN
    using HeaderFunc = void (*)(std::string formatted, std::string header_id);
    using LevelFunc = void (*)();

    void log_set_callbacks(HeaderFunc header, LevelFunc push, LevelFunc pop);
YOSYS_NAMESPACE_END

#endif //LOG_TRACE_H

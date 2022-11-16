#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <windows.h>
#include "glass_export.h"



#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#ifdef _DEBUG
#define MLOG(LOGTYPE, MSG) Debug::Logging::Log(__LINE__, __FILENAME__ ,__FUNCTION__, LOGTYPE, MSG)
#else
#define MLOG(LOGTYPE, MSG) Logging::Log(__LINE__, __FILENAME__ ,__FUNCTION__, LOGTYPE, MSG); //Will change this after finished work
#endif

#define COL_BLUE 9
#define COL_CYAN 11
#define COL_RED 12
#define COL_WHITE 7
#define COL_ORANGE 13
#define COL_GREY

#define LOG_ERROR 0
#define LOG_WARN 1
#define LOG_INFO 2
namespace Debug {

        using LogType = uint8_t;

        struct LogEntry {
            char *time;
            uint32_t line;
            std::string filename;
            std::string function;
            LogType level;
            std::string msg;

        };

//Rewrite this tommorow lucas.
//No.
        struct Logging {
            static inline std::vector<LogEntry> logEntries;

            static void GLASS_EXPORT Log(uint32_t line, const std::string &filename, const std::string &function, LogType level,
                            const std::string &msg);
        };
    }
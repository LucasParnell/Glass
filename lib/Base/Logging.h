#pragma once

#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <windows.h>


#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#ifdef _DEBUG
#define MLOG(LOGTYPE, MSG) Base::Logging::Log(__LINE__, __FILENAME__ ,__FUNCTION__, LOGTYPE, MSG)
#else
#define MLOG(LOGTYPE, MSG) Base::Logging::Log(__LINE__, __FILENAME__ ,__FUNCTION__, LOGTYPE, MSG); //Will change this after finished work
#endif

#define COL_BLUE 9
#define COL_CYAN 11
#define COL_RED 12
#define COL_WHITE 7
#define COL_ORANGE 14


#define LOG_ERROR 0
#define LOG_WARN 1
#define LOG_INFO 2


using LogType = uint8_t;

namespace Base {
    struct Logging {
        static void
        Log(uint32_t line, const std::string &filename, const std::string &function, LogType level,
            const std::string &msg) {
            time_t now = time(0);
            struct tm tm{};
            char buf[80];
            tm = *localtime(&now);
            strftime(buf, sizeof(buf), "%X | ", &tm);

            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

            auto CurrentColour = COL_BLUE;

            switch (level) {
                case LOG_ERROR: {
                    CurrentColour = COL_RED;
                    break;
                }
                case LOG_WARN: {
                    CurrentColour = COL_ORANGE;
                    break;
                }
                default:
                    break;
            }

            std::cout << buf;

            SetConsoleTextAttribute(hConsole, COL_CYAN);
            std::cout << filename;
            SetConsoleTextAttribute(hConsole, COL_WHITE);
            std::cout << " | ";
            SetConsoleTextAttribute(hConsole, COL_CYAN);
            std::cout << "LN" << line;
            SetConsoleTextAttribute(hConsole, COL_WHITE);
            std::cout << " | ";
            SetConsoleTextAttribute(hConsole, COL_CYAN);
            std::cout << function;
            SetConsoleTextAttribute(hConsole, COL_WHITE);
            std::cout << " | ";
            SetConsoleTextAttribute(hConsole, CurrentColour);
            std::cout << msg
                      << std::endl;
            SetConsoleTextAttribute(hConsole, COL_WHITE);
        }
    };
}
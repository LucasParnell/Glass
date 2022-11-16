#include "Logging.h"

void
Debug::Logging::Log(uint32_t line, const std::string &filename, const std::string &function, Debug::LogType level,
                    const std::string &msg) {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X | ", &tstruct);

    LogEntry lastEntry = {(char*)&buf, line, filename, function, level, msg};

    logEntries.push_back(lastEntry);
    if(logEntries.size() > 30){
        std::reverse(logEntries.begin(), logEntries.end());
        logEntries.pop_back();
        std::reverse(logEntries.begin(), logEntries.end());
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    auto CurrentColour = COL_BLUE;

    switch (level) {
        case (LOG_ERROR):
            CurrentColour = COL_RED;
            break;
        case(LOG_WARN):
            CurrentColour = COL_ORANGE;
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
    std::cout << msg << std::endl;
    SetConsoleTextAttribute(hConsole, COL_WHITE);
}


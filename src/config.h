#pragma once
#include <map>
#include <fstream>

template <typename... Args> inline void unused(Args&&...) {}
#define GLOBAL_CONFIG_FILE "chronoserver.cfg"
#define VERSION_1 100000001
#define MAX_MESSAGE_TEXT 100

#define MINUTES_LIMIT 10

class Config
{
    Config() = delete;
    public:
        static void read_global_config();
        static std::map<std::string, std::string> options;
        static std::string get(std::string option);
};

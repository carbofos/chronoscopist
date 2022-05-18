
#include "config.h"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

std::map<std::string, std::string> Config::options;

void Config::read_global_config()
{
    std::ifstream global_config_file (GLOBAL_CONFIG_FILE);
    std::map<std::string, std::string> config;
    if (global_config_file.is_open())
        {
            std::string line;
            while( std::getline(global_config_file, line) )
                {
                std::istringstream iss_line(line);
                std::string key;
                if( std::getline(iss_line, key, '=') )
                {
                    std::string value;
                    if( std::getline(iss_line, value) ) 
                    {
                        Config::options.emplace(key, value);

                    }
                }
            }
        }
        else 
        {
            throw std::runtime_error(std::string("Config file not found: ") + std::string(GLOBAL_CONFIG_FILE) );
        }
	
}

std::string Config::get(std::string option)
{
    if (Config::options.find(option) == Config::options.end())
    {
        std::cerr << "Config error: can't get option " << option << std::endl;
        return "";
    }
    return Config::options.at(option);
}

#pragma once

#include <string>

uint32_t ip_to_long( const std::string& ipv4Str );
std::string long_to_ip(const uint32_t intip);

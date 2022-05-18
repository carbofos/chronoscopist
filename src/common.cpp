#include "common.h"

#include <string>
#include <sstream>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>

uint32_t ip_to_long( const std::string& ipv4Str )
{
    std::istringstream iss( ipv4Str );

    uint32_t ipv4 = 0;

    for( uint32_t i = 0; i < 4; ++i ) {
        uint32_t part;
        iss >> part;
        if ( iss.fail() || part > 255 ) {
            throw std::runtime_error( "Invalid IP address - Expected [0, 255]" );
        }

        // LSHIFT and OR all parts together with the first part as the MSB
        ipv4 |= part << ( 8 * ( 3 - i ) );

        // Check for delimiter except on last iteration
        if ( i != 3 ) {
            char delimiter;
            iss >> delimiter;
            if ( iss.fail() || delimiter != '.' ) {
                throw std::runtime_error( "Invalid IP address - Expected '.' delimiter" );
            }
        }
    }

    return ipv4;
}

std::string long_to_ip( const uint32_t intip)
{
    char buffer[INET_ADDRSTRLEN];
    auto reversed = htonl(intip);
    return inet_ntop(AF_INET, &reversed, buffer, sizeof(buffer));
}

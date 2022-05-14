#pragma once
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include <list>

#include "ChronoServerConnection.h"

using namespace boost::asio;
using namespace boost::posix_time;

io_service service;

class ChronoServer
{
    public:
        ChronoServer() : glob_io_service(&service) {};
        static std::list<ChronoServerConnection_ptr> chronoclients;
        static void handle_accept(ChronoServerConnection_ptr connection, const boost::system::error_code & err);
		static ChronoServerConnection_ptr CreateConnection_ptr();
		io_service *glob_io_service;
};

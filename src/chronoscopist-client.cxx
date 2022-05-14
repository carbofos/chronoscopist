#include <string>
#include <iostream>
#include "ChronoClientConnection.h"

using namespace boost::placeholders;
using namespace boost::asio;
using namespace boost::posix_time;

io_service service;

int main() {
	std::cout << "Chronoscopist - PC usage time control client " << std::endl;
	auto client = std::make_shared<ChronoClientConnection>(service);
	client->connect("192.168.1.3", std::to_string(40004));
	service.run();
	return 0;
}

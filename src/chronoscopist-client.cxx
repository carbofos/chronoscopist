#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "ChronoClientConnection.h"

using namespace boost::asio;
using namespace boost::posix_time;

io_service service;

int main(int argc, char* argv[]) {
	std::cout << "Chronoscopist - PC usage time control client " << std::endl;
	if (argc != 2)
	{
		std::cout << "Usage:  " << argv[0] << " <chronoscopist server address>" << std::endl;
		return 1;
	}

	while ( true )
	{
		std::cout << "Connecting to chronoscopist server at " << argv[1] << std::endl;
		auto client = std::make_shared<ChronoClientConnection>(service);
		client->connect(argv[1], std::to_string(40004));
		service.run();
		// sleep before reconnect (milliseconds)
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	return 0;
}

#include <string>
#include <iostream>
#include "chronoscopist-server.h"
#include "ServerMsgManager.h"
#include "db.h"

using namespace boost::placeholders;
ChronoServer chronoServer;

auto acceptor = ip::tcp::acceptor(*(chronoServer.glob_io_service), ip::tcp::endpoint(ip::tcp::v4(), 40004));

void ChronoServer::handle_accept(ChronoServerConnection_ptr added_customer, const boost::system::error_code & err)
{
	std::cout << "handle_accept\n";
	if (err)
	{
		std::cerr << "handle_accept() error: " << err.message() << std::endl;
		return;
	}

	// TODO: Disconnect client

	// CamClientManager::proxy_customers.remove_if( [] (const BaseCustomerNetworking_ptr &customers)
	// 				{	if (customers->failed_to_connectcam() )
	// 					{
	// 						std::cerr << "Removing from customers as failed to connect. Customers: " << CamClientManager::proxy_customers.size();
	// 						return true;
	// 					}
	// 					return false;
	// 					// return customers->failed_to_connectcam();
	// 				} 
	// );

	added_customer->start();
	ServerMsgManager::add_connection(added_customer);
	ChronoServerConnection_ptr new_customer = ChronoServer::CreateConnection_ptr();
	acceptor.async_accept(new_customer->sock(), boost::bind(&ChronoServer::handle_accept, new_customer, _1));
}


ChronoServerConnection_ptr ChronoServer::CreateConnection_ptr()
{
	return std::make_shared<ChronoServerConnection>();
}

int main() {
	std::cout << "Chronoscopist - PC usage time control server " << std::endl;
	try
	{
		// ChronoServerConnection::camProxy = &camProxy;
		ChronoServerConnection_ptr new_connection = ChronoServer::CreateConnection_ptr();
		// ServerMsgManager::add_connection(new_connection);
		acceptor.async_accept(new_connection->sock(), boost::bind(&ChronoServer::handle_accept, new_connection, _1));
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::thread msgmanager( ServerMsgManager::start );
	chronoServer.glob_io_service->run();
	return 0;
}

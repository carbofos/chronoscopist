#include "ChronoClientConnection.h"

void ChronoClientConnection::connect(const std::string &ip, const std::string &port)
{
    std::cout << "connect()" << std::endl;
	boost::asio::ip::tcp::resolver::query query(ip, port);

	resolver_.async_resolve(query,
		boost::bind(&ChronoClientConnection::handle_resolve, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::iterator));

}

void ChronoClientConnection::handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{
    std::cout << "handle_resolve()" << std::endl;
    if (!err)
    {
        tcp::endpoint endpoint = *endpoint_iterator;
        //   savedendpoint = endpoint;
        socket_.async_connect(endpoint,
                boost::bind(&ChronoClientConnection::handle_connect, shared_from_this(),
                boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        std::cerr << err.message() << std::endl;
        closeconnection();
    }
}

void ChronoClientConnection::handle_connect(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{
    std::cout << "handle_connect()" << std::endl;
    if (!err)
    {
        boost::asio::async_write(socket_, request_,
            boost::bind(&ChronoClientConnection::handle_write_request, shared_from_this(),
              boost::asio::placeholders::error));
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
        closeconnection();
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(endpoint,
            boost::bind(&ChronoClientConnection::handle_connect, shared_from_this(),
              boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        closeconnection();
    }
}

void ChronoClientConnection::handle_write_request(const boost::system::error_code& err)
{
    std::cout << "handle_write_request()" << std::endl;
    socket_.async_write_some( boost::asio::buffer(&write_buffer_, sizeof(write_buffer_)),
            boost::bind(&ChronoClientConnection::on_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );

    closeconnection();
}

void ChronoClientConnection::on_write(const boost::system::error_code & err, const size_t bytes)
{
    if ( err)
    {
        std::cout << "Error in ChronoClientConnection::on_write" << std::endl;
        // stop();
        return;
    }
    // tp_last_operation = std::chrono::steady_clock::now();
    // unused(bytes);
}

void ChronoClientConnection::closeconnection()
{
    std::cout << "closeconnection()" << std::endl;
    boost::system::error_code ec;
    try
        {socket_.cancel();}
    catch (std::exception& e)
        { std::cout << e.what(); }

    try 
        {socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);}
    catch (std::exception& e)
        {std::cout << e.what(); }

    try 
        {socket_.close();}
    catch (std::exception& e)
        {std::cout << e.what(); }
	
}

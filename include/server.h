#ifndef SERVER_H
#define SERVER_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "session.h"
#include "dispatcher.h"

using boost::asio::ip::tcp;

class server
{
public:
	server(boost::asio::io_service& io_service, NginxConfig& config, short port);
	~server();
private:
	void init_config(std::string config_file);
	void start_accept();
	void handle_accept(session* new_session, const boost::system::error_code& error);
	boost::asio::io_service& io_service_;
  	tcp::acceptor acceptor_;
  	Dispatcher* dispatcher_;
	logger log;
};
#endif

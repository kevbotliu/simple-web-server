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
	server(const NginxConfig& config);
	~server();
	void run();
private:
	void start_accept();
	void handle_accept(session* new_session, const boost::system::error_code& error);
	void shutdown();
	boost::asio::io_service io_service_;
	boost::asio::signal_set signals_;
  	tcp::acceptor acceptor_;
  	Dispatcher* dispatcher_;
  	NginxConfig config_;
	logger log;
};
#endif

#ifndef SERVER_H
#define SERVER_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "session.h"
#include <map>
#include "config_parser.h"

using boost::asio::ip::tcp;

class server
{
public:
	server(boost::asio::io_service& io_service, short port, NginxConfig *config);

private:
	void init_handlers();
	void start_accept();
	void handle_accept(session* new_session, const boost::system::error_code& error);
	boost::asio::io_service& io_service_;
  	tcp::acceptor acceptor_;
  	NginxConfig *config_;
  	std::map<std::string, std::vector<std::string>> conf_paths_;
};
#endif

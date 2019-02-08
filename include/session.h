#ifndef SESSION_H
#define SESSION_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <request.h>
#include <response.h>
#include <map>
#include "logger.h"

using boost::asio::ip::tcp;

class session
{
public:
	session(boost::asio::io_service& io_service, std::map<std::string, std::vector<std::string>> conf_paths);
	tcp::socket& socket();
	void start();

private:
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);
	bool run_handler(Request *req, Response *resp);
	tcp::socket socket_;
  	enum { max_length = 1024 };
  	char data_[max_length];
  	std::map<std::string, std::vector<std::string>> conf_paths_;
	logger log;
};
#endif

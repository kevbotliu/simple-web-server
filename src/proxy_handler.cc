#include "proxy_handler.h"
#include "shared.h"

#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/array.hpp>

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>

#include <map>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

RequestHandler* ProxyHandler::create(const NginxConfig& config, const std::string& root_path) {
	// TODO: would like config to have method: getAttribute("remote_url") and "remote_port"
	return new ProxyHandler();
}


std::unique_ptr<Reply> ProxyHandler::HandleRequest(const Request& request) {
	/*
		Author: Konner Macias

		Checks for valid
	*/
	if (!request.is_valid()) return std::unique_ptr<Reply>(nullptr);
	if (request.get_method() != "GET") return std::unique_ptr<Reply>(nullptr);

	// start servicing
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);

	// TODO: this is getting a HOST NOT FOUND (authoritative) error

	// obtains pair (remote_url, remote_port)
	std::pair<std::string,std::string> remote_pair = get_remote_info();
	std::string host = remote_pair.first, port = remote_pair.second;
	
	// send out query
	std::cout << "Host: " << host << " Port: " << port << std::endl;
	tcp::resolver::query query(host, port, boost::asio::ip::resolver_query_base::numeric_service);
	
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

	tcp::resolver::iterator iter = endpoint_iterator;
	tcp::resolver::iterator end;

	// iterate until endpoint
	while (iter != end) {
		tcp::endpoint endpoint = *iter++;
		std::cout << endpoint << std::endl;
	}
	
	// get session/connection
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);

	// build request string
	std::string request_str = "GET " + request.get_path() + " HTTP/1.1\r\n";
	request_str += std::string("Host: ") + host + "\r\n";
	request_str += std::string("Connection: keep-alive\r\n");
	request_str += std::string("Accept: */*\r\n");
	request_str += std::string("Connection: close\r\n\r\n");
	std::cout << request_str << std::endl;

	// send out built request
	socket.write_some(boost::asio::buffer(request_str, request_str.size()));

	// read in response data
	boost::array<char, 2048> buffy;
	boost::system::error_code error;
	size_t len_data = socket.read_some(boost::asio::buffer(buffy), error);

	// check for EOF
	if (error == boost::asio::error::eof) {
		BOOST_LOG_TRIVIAL(trace) << "EOF reached!";
		// HEY: do I throw a nullptr here?
		return std::unique_ptr<Reply>(nullptr);
	}

	// assign response string to incoming data
	std::string returned_response;
	returned_response += std::string(buffy.data(), len_data);

	// define a function to check for invalid response (use if switch to - vars)
	/*
	bool returned_response_check;
	if (!(returned_response_check = parse_returned_response(returned_response))) {
		return std::unique_ptr<Reply>(nullptr);
	}
	*/

	// get pair of (whether it's valid, body of response)
	// -> doing this bc don't wanna use private variables
	std::pair<bool,std::string> parse_pair = parse_returned_response(returned_response);

	// check for invalid response
	if (!parse_pair.first) {
		return std::unique_ptr<Reply>(nullptr);
	}
	std::string response_body = parse_pair.second;

	// complete final response
	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/plain"));
	args.body = response_body;

	return std::unique_ptr<Reply>(new Reply(args));
}


std::pair<bool,std::string> ProxyHandler::parse_returned_response(std::string response) {
	/*
		Author: Konner Macias

		Parses and Confirms a response in valid.
	*/
	std::size_t status_index;
	if ((status_index = response.find("\r\n")) == std::string::npos) {
		return std::pair<bool,std::string> (false, "");
	}

	std::string remainder = response.substr(0, status_index);

	std::size_t header_index;
	if ((header_index = remainder.find("\r\n\r\n")) == std::string::npos) {
		return std::pair<bool,std::string> (false, "");
	}

	std::string response_status = response.substr(0, status_index + 3);
	std::string response_body = response.substr(0, header_index + 4);

	// TODO: add checks for validity on these strings, continue on
	return std::pair<bool,std::string> (true, response_body);
}

std::pair<std::string,std::string> ProxyHandler::get_remote_info() {
	/*
		Author: Konner Macias
		
		Gathers corresponding remote url and remote port for redirect
	*/
	for (auto handler : HandlerInfo::handler_blocks) {
		if (handler.name == "ucla") {
			return std::pair<std::string,std::string> (handler.remote_url, handler.remote_port);
		}
	}
}

#include "proxy_handler.h"

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
	return new ProxyHandler(config);
}

std::unique_ptr<Reply> ProxyHandler::HandleRequest(const Request& request) {
	/*
		Author: Konner Macias

		Checks for valid request, creates new session, sends out response
		to new remote url and port. Get's responsea and checks is complete.
	*/
	if (!request.is_valid()) return std::unique_ptr<Reply>(nullptr);
	if (request.get_method() != "GET") return std::unique_ptr<Reply>(nullptr);

	// start servicing
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);

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
		std::cout << "Endpoint: " << endpoint << std::endl;
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

	std::cout << "Successfully wrote data" << std::endl;

	// read in response data
	std::string returned_response;
	while(1) {
		boost::array<char, 4096> buffy;
		boost::system::error_code error;
		size_t len_data = socket.read_some(boost::asio::buffer(buffy), error);
		
		// check for EOF
		if (error == boost::asio::error::eof) {
			BOOST_LOG_TRIVIAL(trace) << "EOF reached!";
			break;
		}

		// assign response string to incoming data
		returned_response += std::string(buffy.data(), len_data);
	}

	std::cout << "Received response string, parsing..." << std::endl;

	// get the response in parsed format
	std::map<std::string,std::string> parse_map = parse_returned_response(returned_response);

	// check for invalid response
	if (parse_map["Valid"] == "false") {
		return std::unique_ptr<Reply>(nullptr);
	}
	std::string response_body = parse_map["Body"];

	// complete final response
	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));
	args.headers.push_back(std::make_pair("Content-length", std::to_string(response_body.length())));
	args.body = response_body;
	std::cout << "Completed building final response" << std::endl;

	return std::unique_ptr<Reply>(new Reply(args));
}


std::map<std::string,std::string> ProxyHandler::parse_returned_response(std::string response) {
	/*
		Author: Konner Macias

		Parses and Confirms a response in valid. Returns a map with possible data members:
			- Valid
			- Status
			- Headers
			- Body
	*/
	std::map<std::string,std::string> response_map;
	response_map["Valid"] = "true";

	std::size_t status_index;
	if ((status_index = response.find("\r\n")) == std::string::npos) {
		response_map["Valid"] = "false";
		return response_map;
	}

	std::size_t header_index;
	if ((header_index = response.find("\r\n\r\n")) == std::string::npos) {
		response_map["Valid"] = "false";
		return response_map;
	}

	// TODO: got some magic numbers here
	response_map["Status"] = response.substr(0, status_index + 1);
	response_map["Headers"] = response.substr(0, header_index);
	response_map["Body"] = response.substr(header_index + 11, response.size());

	// TODO: add checks further checks for validity on these strings, continue on
	return response_map;
}

std::pair<std::string,std::string> ProxyHandler::get_remote_info() {
	/*
		Author: Konner Macias
		
		Gathers corresponding remote url and remote port for redirect
	*/
	for (auto handler : config_.handler_blocks) {
		if (handler.name == "ucla") {
			return std::pair<std::string,std::string> (handler.remote_url, handler.remote_port);
		}
	}
}

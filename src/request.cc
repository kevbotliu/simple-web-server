/*
 * Request class representing a single HTTP request.
 * Breaks down raw request string into components.
 * Makes no assumptions about validity of fields, only syntax.
 */

#include "request.h"
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <regex>
#include <iostream>

Request::Request(std::string request_str)
	: raw_(request_str)
{
	valid_ = parse();
}

bool Request::parse()
{
	std::string s = raw_;

	int crlf_in;

	// Request line
	if ((crlf_in = s.find("\r\n")) != std::string::npos) {
		std::vector<std::string> statuses;
		std::string rline = s.substr(0, crlf_in);
  	boost::split(statuses, rline, boost::is_any_of(" "));

  	s = s.erase(0, crlf_in);

  	if (statuses.size() == 3) {
  		method_ = statuses[0];
	  	path_ = statuses[1];
	  	version_ = statuses[2];
  	}
  	else return false;
	}

	// Check valid no headers
	if (s.size() >= 4 && s.substr(0, 4) == "\r\n\r\n") {
		if (s.size() > 4) body_ = s.substr(4);
		return true;
	}

	// Headers
	if (!s.empty() && (s.find("\r\n") != std::string::npos)) {
		
	  std::smatch header_match;
		std::regex header_regex("\\\r\\\n(.+?: .+?(?=\\\r\\\n))");
		while (std::regex_search(s, header_match, header_regex)) {
			std::string h = header_match.str(1);
			int delim = h.find(": ");
	    headers_.push_back(std::make_pair(h.substr(0, delim), h.substr(delim + 2)));

		  s = header_match.suffix();
		}

		// Check empty line
		if (s.size() >= 4 && s.substr(0, 4) == "\r\n\r\n") {
			if (s.size() > 4) body_ = s.substr(4);
			return true;
		}
	}

	return false;
}

bool Request::is_valid_syntax() {return valid_;}

std::string Request::get_method() {return method_;}
std::string Request::get_path() {return path_;}
std::string Request::get_version() {return version_;}
std::vector<std::pair<std::string, std::string>> Request::get_headers() {return headers_;}
std::string Request::get_body() {return body_;}
std::string Request::get_raw() {return raw_;}



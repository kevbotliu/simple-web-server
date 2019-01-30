/*
 * Request class representing a single HTTP request.
 * Breaks down raw request string into components.
 * Makes no assumptions about validity of fields, only syntax.
 */


#include "request.h"
#include <boost/algorithm/string.hpp>
#include <regex>

Request::Request(std::string request_str)
	: raw_(request_str),
		valid_(false)
{
	parse();
}

void Request::parse()
{
	std::smatch rline_match;
	std::regex rline_regex("^([A-Z]+? \\S+ HTTP\\/\\d.\\d)\\\r\\\n.+");

	// Request line
  if (std::regex_search(raw_, rline_match, rline_regex)) {
  	std::vector<std::string> statuses;
  	std::string rline = rline_match.str(1);
  	boost::split(statuses, rline, boost::is_any_of(" "));

  	method_ = statuses[0];
  	path_ = statuses[1];
  	version_ = statuses[2];
  }

  std::string s = raw_;

  // Headers
  std::smatch header_match;
	std::regex header_regex("\\\r\\\n(.+?: .+?(?=\\\r\\\n))");
	while (std::regex_search(s, header_match, header_regex)) {
		std::string h = header_match.str(1);
		int delim = h.find(": ");
    headers_.push_back(std::make_pair(h.substr(0, delim), h.substr(delim + 2)));

	  s = header_match.suffix();
	}
	s = s.substr(2);

    // Check extra \r\n
	if (s.substr(0,2) == "\r\n") valid_ = true;

	// Request body (for future projects?)
	if (s.size() > 2) body_ = s.erase(0, 2);
}

bool Request::is_valid_syntax() {return valid_;}

std::string Request::get_method() {return method_;}
std::string Request::get_path() {return path_;}
std::string Request::get_version() {return version_;}
std::vector<std::pair<std::string, std::string>> Request::get_headers() {return headers_;}
std::string Request::get_body() {return body_;}
std::string Request::get_raw() {return raw_;}



#include "response.h"
#include <algorithm>
#include <iostream>

Response::Response()
	: version_(),
		status_(),
		headers_(),
		body_()
{}

void Response::set_version(std::string version) { version_ = version; }

bool Response::set_status(int code)
{
	if (status_msgs.find(code) == status_msgs.end()) {
		std::cout << "Invalid status code attempt to set" << std::endl;
		return false;
	}
	
	status_ = std::to_string(code) + " " + status_msgs[code];
	return true;
}

void Response::set_headers(std::vector<std::pair<std::string, std::string>> headers) { headers_ = headers; }

void Response::add_header(std::pair<std::string, std::string> header) { headers_.push_back(header); }

// Untested and probably buggy!
std::pair<std::string, std::string> Response::remove_header(int index)
{
	if (headers_.size() < index || index < 0) return NULL;

	std::vector<std::pair<std::string, std::string>> old_header = headers_[index];
	headers_.erase(headers_.begin() + index);
	return old_header;
}

void Response::set_body(std::string body_str) { body_ = body_str; }

std::string Response::to_string()
{
	std::string output;
	output = version_ + " " + status_ + "\r\n";

	for (std::pair<std::string, std::string> header : headers_) {
		output += (header.first + ": " + header.second + "\r\n");
	}

	output += "\r\n";
	output += body_;

	return output;
}

int Response::size() {return to_string().size();}

#include "reply.h"
#include <algorithm>
#include <iostream>

Reply::Reply(ReplyArgs args) 
	: valid_(true) {
	if (args.HTTP_version.size() < 6 || args.HTTP_version.substr(0,5) != "HTTP/") {
		std::cout << "Invalid HTTP version syntax" << std::endl;
		valid_ = false;
	}
	else version_ = args.HTTP_version;

	if (status_msgs.find(args.status_code) == status_msgs.end()) {
		std::cout << "Invalid status code set" << std::endl;
		valid_ = false;
	}
	else status_ = args.status_code;

	headers_ = args.headers;
	body_ = args.body;
}

Reply::Reply(bool is_default=true) 
	: valid_(true) {
	version_ = "HTTP/1.1";
	if (is_default) {
		status_ = 200;
		body_ = "";
	}
	else {
		status_ = 404;
		body_ = "404 Not Found";
	}
}

int Reply::get_status_code() {return status_;}

std::string Reply::to_string() {
	if (!valid_) return "";

	std::string status_line = version_ + " " + std::to_string(status_) + " " + status_msgs[status_] + "\r\n";
	std::string header_lines = "";
	for (auto header : headers_) {
		header_lines += (header.first + ": " + header.second + "\r\n");
	}
	return status_line + header_lines + "\r\n" + body_;
}

std::string Reply::get_body() {
	if (!valid_) return "";

	return body_;
}

bool Reply::is_valid() { return valid_; }
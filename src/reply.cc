#include "reply.h"
#include <algorithm>
#include <iostream>

Reply::Reply(ReplyArgs args) 
	: valid_(true) {
	version_ = args.HTTP_version;

	if (status_msgs.find(args.status_code) == status_msgs.end()) {
		std::cout << "Invalid status code set" << std::endl;
		valid_ = false;
	}
	else status_ = args.status_code;

	headers_ = args.headers;
	body_ = args.body;
}

std::string Reply::to_string() {
	if (!valid_) return "";

	std::string status_line = version_ + " " + std::to_string(status_) + " " + status_msgs[status_] + "\r\n";
	std::string header_lines = "";
	for (auto header : headers_) {
		header_lines += (header.first + ": " + header.second + "\r\n");
	}
	return status_line + header_lines + "\r\n" + body_;
}

bool Reply::is_valid() { return valid_; }
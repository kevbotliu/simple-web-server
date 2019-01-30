#ifndef RESPONSE_H
#define RESPONSE_H

#include <vector>
#include <map>
#include <utility>
#include <string>

// Add more when necessary
static std::map<int, std::string> status_msgs = {
  {200, "OK"}
};

class Response
{
public:
	Response();

	void set_version(std::string version);
	bool set_status(int code);
	void set_headers(std::vector<std::pair<std::string, std::string>> headers);
	void add_header(std::pair<std::string, std::string> header);
	// std::pair<std::string, std::string> remove_header(int index);
	void set_body(std::string body_str);
	std::string to_string();
	int size();

	void invalidate(); // Temporary!

private:
	std::string version_;
	std::string status_;
	std::vector<std::pair<std::string, std::string>> headers_;
	std::string body_;

	bool valid_; // Temporary!
};
#endif
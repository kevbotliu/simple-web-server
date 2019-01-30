#ifndef REQUEST_H
#define REQUEST_H

#include <vector>
#include <utility>
#include <string>

class Request
{
public:
	Request(std::string request_str);

	bool is_valid_syntax();

	std::string get_method();
	std::string get_path();
	std::string get_version();
	std::vector<std::pair<std::string, std::string>> get_headers();
	std::string get_body();
	std::string get_raw();

private:
	void parse();

	bool valid_;
	std::string raw_;
	std::string method_;
	std::string path_;
	std::string version_;
	std::vector<std::pair<std::string, std::string>> headers_;
	std::string body_;
};
#endif
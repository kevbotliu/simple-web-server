#ifndef REQUEST_H
#define REQUEST_H

#include <vector>
#include <utility>
#include <string>

class Request {
public:
	Request(std::string request_str);

	std::string get_raw() const;

	std::string get_method() const;
	std::string get_path() const;
	std::string get_version() const;
	std::vector<std::pair<std::string, std::string>> get_headers() const;
	std::string get_body() const;
	
	bool is_valid() const;
private:
	bool parse();
	std::string raw_;
	
	std::string method_;
	std::string path_;
	std::string version_;
	std::vector<std::pair<std::string, std::string>> headers_;
	std::string body_;

	bool valid_;
};
#endif
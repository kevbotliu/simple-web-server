#ifndef REPLY_H
#define REPLY_H

#include <vector>
#include <map>
#include <utility>
#include <string>

// Add more when necessary
static std::map<int, std::string> status_msgs = {
  {200, "OK"},
  {404, "Not Found"}
};

typedef struct {
	std::string HTTP_version = "HTTP/1.1";
	int status_code = 200;
	std::vector<std::pair<std::string, std::string>> headers;
	std::string body = "";
} ReplyArgs;

class Reply {
public:
	Reply(ReplyArgs args);

	std::string to_string();
	bool is_valid();
private:
	std::string version_;
	int status_;
	std::vector<std::pair<std::string, std::string>> headers_;
	std::string body_;
	bool valid_;
};
#endif
#include "echo_handler.h"

RequestHandler* EchoHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new EchoHandler();
}

std::unique_ptr<Reply> EchoHandler::HandleRequest(const Request& request) {
	if (!request.is_valid()) {
		return std::unique_ptr<Reply>(nullptr);
	}

	ReplyArgs args;
	if (request.get_method() != "GET") {
		return std::unique_ptr<Reply>(nullptr);
	}

	std::pair<std::string, std::string> header = std::make_pair("Content-type", "text/plain");
	args.headers.push_back(header);

	args.body = request.get_raw();

	return std::unique_ptr<Reply>(new Reply(args));
	
}
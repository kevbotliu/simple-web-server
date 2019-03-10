#include "health_handler.h"

RequestHandler* HealthHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new HealthHandler();
}

std::unique_ptr<Reply> HealthHandler::HandleRequest(const Request& request) {
	if (request.get_method() != "GET") return std::unique_ptr<Reply>(new Reply(false));

	ReplyArgs args;
	args.body = "OK";
	return std::unique_ptr<Reply>(new Reply(args));
}
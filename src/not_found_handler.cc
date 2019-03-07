#include "not_found_handler.h"

RequestHandler* NotFoundHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new NotFoundHandler();
}

std::unique_ptr<Reply> NotFoundHandler::HandleRequest(const Request& request) {
	return std::unique_ptr<Reply>(new Reply(false));
}
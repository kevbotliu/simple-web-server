#include "not_found_handler.h"

RequestHandler* NotFoundHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new NotFoundHandler();
}

std::unique_ptr<Reply> NotFoundHandler::HandleRequest(const Request& request) {
	ReplyArgs args;
	args = build_response(request);
	return std::unique_ptr<Reply>(new Reply(args));
}

ReplyArgs NotFoundHandler::build_response(const Request& request)
{
	ReplyArgs args;
	std::pair<std::string, std::string> header = std::make_pair("Content-type", "text/html");
	
	args.body = std::string("404 Not Found");
	args.status_code = 404;

	return args;
}

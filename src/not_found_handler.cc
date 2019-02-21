#include "not_found_handler.h"

RequestHandler* NotFoundHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new NotFoundHandler();
}

std::unique_ptr<Reply> NotFoundHandler::HandleRequest(const Request& request) {
	ReplyArgs args;

	//resp_->set_version(req_->get_version());
	//resp_->set_status(404);

	std::pair<std::string, std::string> header = std::make_pair("Content-type", "text/html");
	//resp_->add_header(header);

	args.body = std::string("404 Not Found");
	std::cerr << "Could not open file.";

	return std::unique_ptr<Reply>(new Reply(args));
}

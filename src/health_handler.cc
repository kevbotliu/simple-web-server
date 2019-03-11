#include "health_handler.h"

RequestHandler* HealthHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new HealthHandler();
}

std::unique_ptr<Reply> HealthHandler::HandleRequest(const Request& request) {
	if (request.get_method() != "GET") return std::unique_ptr<Reply>(new Reply(false));

	ReplyArgs args;
	args.body = "OK";

	std::unique_ptr<Reply> rep = std::unique_ptr<Reply>(new Reply(args));
	std::string message = "Health Handler::ResponseMetrics::Status Code:" + std::to_string(rep->get_status_code());
	log.log(message, boost::log::trivial::info);

	return rep;
}
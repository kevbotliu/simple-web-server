#include "echo_handler.h"
#include "logger.h"

RequestHandler* EchoHandler::create(const NginxConfig& config, const std::string& root_path){
	return new EchoHandler();
}

std::unique_ptr<Reply> EchoHandler::HandleRequest(const Request& request) {
	if (request.get_method() != "GET") return std::unique_ptr<Reply>(new Reply(false));

	ReplyArgs args;
	std::pair<std::string, std::string> header = std::make_pair("Content-type", "text/plain");
	args.headers.push_back(header);
	args.body = request.get_raw();

	std::unique_ptr<Reply> rep = std::unique_ptr<Reply>(new Reply(args));

	std::string message = "Echo Handler::ResponseMetrics::Status Code:" + std::to_string(rep->get_status_code());

	log.log(message, boost::log::trivial::info);

	return rep;
}
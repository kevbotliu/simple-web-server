#include "status_handler.h"
#include "shared.h"

RequestHandler* StatusHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new StatusHandler(config);
}

std::unique_ptr<Reply> StatusHandler::HandleRequest(const Request& request) {
	if (request.get_method() != "GET") return std::unique_ptr<Reply>(new Reply(false));

	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/plain"));
	args.body += "Number of requests received: " + std::to_string(RequestHistory::history.size()) + "\n\n";
	args.body += "Received requests:\n";
	for (auto req : RequestHistory::history) {
		args.body += req.first + ", " + std::to_string(req.second) + "\n";
	}
	args.body += "\n";
	args.body += "Request handlers registered:\n";
	for (auto handler : config_.handler_blocks) {
		args.body += handler.name + ": " + handler.path + "\n";
	}
	
	return std::unique_ptr<Reply>(new Reply(args));
	
}
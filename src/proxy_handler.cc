#include "proxy_handler.h"


RequestHandler* ProxyHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new ProxyHandler();
}

std::unique_ptr<Reply> ProxyHandler::HandleRequest(const Request& request) {
    
    //TODO: Proxy functionality

    /*
    Echo Handler as reference
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
    */
	
}
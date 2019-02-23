#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H

#include "request_handler.h"

class ProxyHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);
	std::unique_ptr<Reply> HandleRequest(const Request& request) override;
private:
	std::pair<bool,std::string> parse_returned_response(std::string response);
	ProxyHandler() {}    //TODO: Perhaps need more variables to hold config stuff for the proxy

};
#endif
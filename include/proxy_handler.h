#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H

#include "request_handler.h"

class ProxyHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);
	std::unique_ptr<Reply> HandleRequest(const Request& request) override;
private:
	NginxConfig config_;
	ProxyHandler(const NginxConfig& config) : config_(config) {}    //TODO: Perhaps need more variables to hold config stuff for the proxy

	std::map<std::string,std::string> parse_returned_response(std::string response);
	std::tuple<std::string,std::string,std::string> get_remote_info();
	
};
#endif
#ifndef STATUS_HANDLER_H
#define STATUS_HANDLER_H

#include "request_handler.h"
#include "server.h"

class StatusHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);
	std::unique_ptr<Reply> HandleRequest(const Request& request) override;
private:
	NginxConfig config_;
	StatusHandler(const NginxConfig& config) : config_(config) {}
};
#endif
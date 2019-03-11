#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "request_handler.h"
#include "logger.h"

class EchoHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);
	std::unique_ptr<Reply> HandleRequest(const Request& request) override;
private:
	EchoHandler() : log() {}
	logger log;
};
#endif
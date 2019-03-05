#ifndef MEME_HANDLER_H
#define MEME_HANDLER_H

#include "request_handler.h"
#include "server.h"

class MemeHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);
	std::unique_ptr<Reply> HandleRequest(const Request& request) override;
private:
	NginxConfig config_;
	MemeHandler(const NginxConfig& config) : config_(config) {}
};
#endif

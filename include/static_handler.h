#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include "request_handler.h"
#include "pathfinder.h"

class StaticHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);

	std::unique_ptr<Reply> HandleRequest(const Request& request) override;

private:
	std::string root_path_;
	StaticHandler(std::string root_path) : root_path_(root_path) {}
	std::string ext;

	ReplyArgs build_response(const Request& request);
};
#endif

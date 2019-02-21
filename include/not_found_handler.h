#ifndef NOT_FOUND_HANDLER_H
#define NOT_FOUND_HANDLER_H

#include "request_handler.h"

class NotFoundHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);
	std::unique_ptr<Reply> HandleRequest(const Request& request) override;

private:
	NotFoundHandler() {};
	ReplyArgs build_response(const Request& request);
};
#endif

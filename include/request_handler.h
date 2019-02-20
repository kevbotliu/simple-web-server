#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "request.h"
#include "reply.h"
#include "config_parser.h"
#include <memory>

class RequestHandler {
public:
	virtual std::unique_ptr<Reply> HandleRequest(const Request& request) = 0;
protected:
	NginxConfig config;
	std::string root_path_;
};
#endif
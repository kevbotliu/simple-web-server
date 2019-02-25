#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "handler_factory.h"

class Dispatcher {
public:
	Dispatcher(const NginxConfig& config)
		: config_(config), factory_(HandlerFactory()) {}
	std::unique_ptr<RequestHandler> dispatch(Request& req);
private:
	NginxConfig config_;
	HandlerFactory factory_;
};
#endif
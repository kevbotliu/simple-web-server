#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "handler_factory.h"

class Dispatcher {
public:
	static Dispatcher* create(const NginxConfig& config);
	std::unique_ptr<RequestHandler> dispatch(Request& req);
private:
	Dispatcher(const NginxConfig& config) : config_(config) {}
	void extract();
	
	NginxConfig config_;
	HandlerFactory factory_;
};
#endif
#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "handler_factory.h"

typedef struct {
	std::string name;
	std::string path;
	std::string root_path;
} HandlerBlock;

class Dispatcher {
public:
	static Dispatcher* create(const NginxConfig& config);
	std::unique_ptr<RequestHandler> dispatch(Request& req);
	
	std::vector<HandlerBlock> handler_blocks;
private:
	Dispatcher(const NginxConfig& config) : config_(config) {}
	void extract();

	NginxConfig config_;
	HandlerFactory factory_;
};
#endif
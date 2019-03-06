#include "dispatcher.h"	
#include "logger.h"
#include "shared.h"
#include <iostream>

// Refuses to build without this??
// HandlerFactory implementation linking fails
#include "handler_factory.cc"

std::unique_ptr<RequestHandler> Dispatcher::dispatch(Request& req) {
	for (auto handler : config_.handler_blocks) {
		// All paths will go into factory if 404 handler is registered
		if (req.get_path().find(handler.path) == 0) {
			std::cout << "Creating handler: " << handler.name << " Path: " << handler.path << std::endl;
			return factory_.createByName(handler.name, config_, handler.root_path);
		}
	}
}




#ifndef HANDLER_FACTORY_H
#define HANDLER_FACTORY_H

#include "request_handler.h"
#include <memory>
#include <iostream>

class HandlerFactory {
public:
	std::unique_ptr<RequestHandler> createByName(const std::string& name, const NginxConfig& config, const std::string& root_path);
};
#endif
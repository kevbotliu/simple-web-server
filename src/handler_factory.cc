#include "handler_factory.h"
#include "echo_handler.h"
#include "static_handler.h"
#include "status_handler.h"
#include "not_found_handler.h"
#include "pathfinder.h"

std::unique_ptr<RequestHandler> HandlerFactory::createByName(const std::string& name, const NginxConfig& config, const std::string& root_path) {
    if (name == "echo") return std::unique_ptr<RequestHandler>(EchoHandler::create(config, root_path));
    if (name == "static") return std::unique_ptr<RequestHandler>(StaticHandler::create(config, root_path));
    if (name == "status") return std::unique_ptr<RequestHandler>(StatusHandler::create(config, root_path));
    
    return std::unique_ptr<RequestHandler>(NotFoundHandler::create(config, root_path));
}









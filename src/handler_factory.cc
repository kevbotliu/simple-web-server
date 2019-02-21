#include "handler_factory.h"
#include "echo_handler.h"
#include "static_handler.h"
#include "status_handler.h"
// #include "404_handler.h"

std::unique_ptr<RequestHandler> HandlerFactory::createByName(const std::string& name, const NginxConfig& config, const std::string& root_path) {
	if (name == "echo") return std::unique_ptr<RequestHandler>(EchoHandler::create(config, root_path));
    if (name == "static") return std::unique_ptr<RequestHandler>(StaticHandler::create(config, root_path));
    // if (name == "404") return 404Handler::create(config, path);
    if (name == "status") return std::unique_ptr<RequestHandler>(StatusHandler::create(config, root_path));

    return std::unique_ptr<RequestHandler>(nullptr);
}









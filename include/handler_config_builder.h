#ifndef HANDLER_CONFIG_BUILDER_H
#define HANDLER_CONFIG_BUILDER_H

#include "handler_config.h"

class HandlerConfigBuilder
{
public:
	HandlerConfigBuilder() : conf_filename_("../server_config_cloud/") {}

	HandlerConfigBuilder& set_config(std::string filename);
	HandlerConfigBuilder& register_handler(std::string handler_name);
	HandlerConfig* build();

private:
	std::string conf_filename_;
	std::vector<Handler> handlers_;
	
};
#endif

#ifndef HANDLER_CONFIG_H
#define HANDLER_CONFIG_H

#include <string>
#include <vector>
#include <config_parser.h>
#include "logger.h"

typedef struct {
	std::string name;
	std::vector<std::string> paths;
	std::string root_dir;
} Handler;


class HandlerConfig
{
public:
	HandlerConfig(std::string config, std::vector<Handler> handlers)
		: conf_filename_(config),
			handlers_(handlers),
			log() {}

	bool extract();
	short get_server_port();
	std::vector<Handler> handlers_;
private:
	short port_;
	std::string conf_filename_;
	NginxConfig config_;
	logger log;
	
};
#endif

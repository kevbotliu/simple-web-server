#include "handler_config.h"

bool HandlerConfig::extract() {
	NginxConfigParser parser;
  if (!parser.Parse(conf_filename_.c_str(), &config_)){
    log.log("Main: Missing or malformed configuration file detected", boost::log::trivial::error);
    return false;
  }

  port_ = parser.getPort();

	log.log("Server: Initializing Handlers...", boost::log::trivial::info);

	for (const auto& statement : config_.statements_) {
		for (auto it = handlers_.begin(); it != handlers_.end(); it++) {
			if (statement->tokens_[0] == "path" && 
					statement->tokens_[2] == it->name &&
					statement->tokens_.size() == 3) {

				for (auto& child_statement : statement->child_block_->statements_) {
					if (child_statement->tokens_[0] == "root") {
						it->root_dir = child_statement->tokens_[1];
					}
				}

				it->paths.push_back(statement->tokens_[1]);
			}
		}
		
	}

	return true;
}

short HandlerConfig::get_server_port() { return port_; }

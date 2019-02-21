#include "dispatcher.h"	
#include "logger.h"
#include "shared.h"
#include <iostream>

// Refuses to build without this??
// HandlerFactory implementation linking fails
#include "handler_factory.cc"

Dispatcher* Dispatcher::create(const NginxConfig& config) {
	Dispatcher* d = new Dispatcher(config);
	d->extract();
	d->factory_ = HandlerFactory();
	return d;
}


void Dispatcher::extract() {
	// log.log("Server: Initializing Handlers...", boost::log::trivial::info);

	for (const auto& statement : config_.statements_) {
		if (statement->tokens_[0] == "#") continue;

		if (statement->tokens_[0] == "handler" && 
			statement->tokens_.size() == 2 &&
			statement->child_block_ != nullptr) {

			HandlerBlock block;
			block.name = statement->tokens_[1];
			for (auto& child_statement : statement->child_block_->statements_) {
				if (child_statement->tokens_.size() == 2 &&
					child_statement->tokens_[0] == "location") {
					block.path = child_statement->tokens_[1];
				}

				if (child_statement->tokens_.size() == 2 &&
					child_statement->tokens_[0] == "root") {
					block.root_path = child_statement->tokens_[1];
				}
			}
			HandlerInfo::handler_blocks.push_back(block);
		}
	}
}



// TODO:
// Config edge cases: 

// In the event that locations for multiple handlers could match a request, 
// the dispatcher should dispatch to the handler matching the most-specific (i.e. longest) 
// configured location. Hint: check handler location matches in order of descending length

// In the event of two handlers being registered to the same location, the first one 
// registered in the config takes priority
std::unique_ptr<RequestHandler> Dispatcher::dispatch(Request& req) {
	for (auto handler : HandlerInfo::handler_blocks) {
		if (req.get_path().find(handler.path) != std::string::npos) {
		    // First check if the file exists.
		    if (access(req.get_path().c_str(), F_OK) != -1) {
			// If file exists, go to static handler.
			return factory_.createByName(handler.name, config_, handler.root_path);
		    }
		    else {
			// If the file doesn't exist, give a 404 message.
		        return factory_.createByName("404", config_, handler.root_path);
		    }
		}
	}
	return std::unique_ptr<RequestHandler>(nullptr);
}




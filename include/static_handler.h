#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include "request_handler.h"
#include "pathfinder.h"
#include <boost/thread.hpp>

class StaticHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);

	std::unique_ptr<Reply> HandleRequest(const Request& request) override;

private:
	NginxConfig config_;
	std::string root_path_;
	StaticHandler(const NginxConfig& config, std::string root_path) : config_(config), root_path_(root_path) {}
	std::string ext;

	boost::mutex mutex;

	ReplyArgs build_response(const Request& request);
};
#endif

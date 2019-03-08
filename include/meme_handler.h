#ifndef MEME_HANDLER_H
#define MEME_HANDLER_H

#include <boost/thread.hpp>
#include "request_handler.h"
#include "server.h"

class MemeHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);
	std::unique_ptr<Reply> HandleRequest(const Request& request) override;
private:
	NginxConfig config_;
	std::string root_path_;
	MemeHandler(const NginxConfig& config, std::string root_path) : config_(config), root_path_(root_path) {}

	boost::mutex mutex;

	std::unique_ptr<Reply> handleView(int id);
	std::unique_ptr<Reply> handleList();
	std::unique_ptr<Reply> handleNew();
};
#endif

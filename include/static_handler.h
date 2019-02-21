#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include "request_handler.h"

class StaticHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);

	std::unique_ptr<Reply> HandleRequest(const Request& request) override;

private:
	std::string ext;
	StaticHandler(std::string root_path);

	void setExtension(std::string path);
	std::string findFullPath(std::string path);
	ReplyArgs build_response(const Request& request);
};
#endif
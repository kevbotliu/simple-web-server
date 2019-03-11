#ifndef MEME_HANDLER_H
#define MEME_HANDLER_H

#include <boost/thread.hpp>
#include "request_handler.h"
#include "server.h"
#include "logger.h"

typedef std::map<std::string,std::string> ParamMap;

class MemeHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, const std::string& root_path);
	std::unique_ptr<Reply> HandleRequest(const Request& request) override;
private:
	NginxConfig config_;
	std::string root_path_;
	MemeHandler(const NginxConfig& config, std::string root_path) : config_(config), root_path_(root_path) , log() {}

	boost::mutex mutex;

	ParamMap extract_params(const Request& request);

	std::unique_ptr<Reply> handleNew();
	std::unique_ptr<Reply> handleCreate(ParamMap& params);
	std::unique_ptr<Reply> handleView(ParamMap& params);
	std::unique_ptr<Reply> handleList(ParamMap& params);
	std::unique_ptr<Reply> handleEdit();
	std::unique_ptr<Reply> handleUpdate(ParamMap& params);
	std::unique_ptr<Reply> handleDelete(ParamMap& params);

	logger log;
};
#endif

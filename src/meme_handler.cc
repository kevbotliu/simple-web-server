#include "meme_handler.h"
#include "shared.h"

RequestHandler* MemeHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new MemeHandler(config);
}

std::unique_ptr<Reply> MemeHandler::HandleRequest(const Request& request) {
	if (!request.is_valid()) return std::unique_ptr<Reply>(nullptr);
	if (request.get_method() != "GET") return std::unique_ptr<Reply>(nullptr);

	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/plain"));
	args.body = "MEMES MEMES MEMES";
	
	return std::unique_ptr<Reply>(new Reply(args));
	
}

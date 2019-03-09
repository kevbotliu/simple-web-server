#include "static_handler.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>


RequestHandler* StaticHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new StaticHandler(config, root_path);
}

std::unique_ptr<Reply> StaticHandler::HandleRequest(const Request& request) {
	if (request.get_method() != "GET") return std::unique_ptr<Reply>(new Reply(false));

	ReplyArgs args;
	args = build_response(request);

	return std::unique_ptr<Reply>(new Reply(args));
}

ReplyArgs StaticHandler::build_response(const Request& request) {
	ReplyArgs args;

	std::string path = request.get_path();

	Pathfinder pf(path);
	pf.set_extension();
	std::string full_path = pf.find_full_path(config_.server_root_path, root_path_);

	mutex.lock();
	if (access(full_path.c_str(), F_OK) != -1) {

		std::pair<std::string, std::string> header = std::make_pair("Content-type", pf.get_extension());
		args.headers.push_back(header);

		std::ifstream f(full_path.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
			
		std::streampos mSize = f.tellg();
		char* mBuffer = new char[mSize];
		f.seekg(0, std::ios::beg);
		f.read(mBuffer, mSize);
		f.close();

		args.body = std::string(mBuffer, mSize);
		delete mBuffer;
	}
	else {
		args.status_code = 404;
		args.body = "404 Not Found";
	}
	mutex.unlock();
	
	return args;
}

#include "static_handler.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>


RequestHandler* StaticHandler::create(const NginxConfig& config, const std::string& root_path)
{
	return new StaticHandler(root_path);
}

StaticHandler::StaticHandler(std::string root_path) {
	root_path_ = root_path;
}

std::unique_ptr<Reply> StaticHandler::HandleRequest(const Request& request) {
	if (!request.is_valid()) {
		return std::unique_ptr<Reply>(nullptr);
	}

	ReplyArgs args;
	if (request.get_method() != "GET") {
		return std::unique_ptr<Reply>(nullptr);
	}

	args = build_response(request);

	return std::unique_ptr<Reply>(new Reply(args));
	
}

ReplyArgs StaticHandler::build_response(const Request& request)
{
	ReplyArgs args;
	std::string path = request.get_path();

	Pathfinder pathfinder(path);
	std::string pathToCheck = pathfinder.findFullPath(path, root_path_);

	if (access(pathToCheck.c_str(), F_OK) != -1) {
		// resp_->set_version(req_->get_version());
		// resp_->set_status(200);
		// std::pair<std::string, std::string> header = std::make_pair("Content-type", ext);
		// resp_->add_header(header);

		std::pair<std::string, std::string> header = std::make_pair("Content-type", ext);
		args.headers.push_back(header);

		std::string full_path = pathfinder.findFullPath(path, root_path_);
		std::ifstream f(full_path.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
			
		std::streampos mSize = f.tellg();
		char* mBuffer = new char[mSize];
		f.seekg(0, std::ios::beg);
		f.read(mBuffer, mSize);
		f.close();

		// resp_->set_version(req_->get_version());
		// resp_->set_status(200);
		// std::pair<std::string, std::string> header = std::make_pair("Content-type", ext);
		// resp_->add_header(header);
		// resp_->set_body(std::string(mBuffer, mSize));
		args.body = std::string(mBuffer, mSize);
		delete mBuffer;
	}
	else {
		args.status_code = 404;
		args.body = "404 Not Found";
	}
	
	return args;
}

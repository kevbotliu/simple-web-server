#include "static_handler.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


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
	setExtension(path);

	// resp_->set_version(req_->get_version());
	// resp_->set_status(200);
	// std::pair<std::string, std::string> header = std::make_pair("Content-type", ext);
	// resp_->add_header(header);

	std::pair<std::string, std::string> header = std::make_pair("Content-type", ext);
	args.headers.push_back(header);

	std::string full_path = findFullPath(path);
	std::ifstream f(full_path.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
	if (f.is_open()) {
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
		return args;
	} else {
		// resp_->set_version(req_->get_version());
		// resp_->set_status(404);
		// std::pair<std::string, std::string> header = std::make_pair("Content-type", "text/html");
		// resp_->add_header(header);
		args.body = std::string("404 Not Found");
		std::cerr << "Could not open file.";
		return args;
	}
	return args;
}

std::string StaticHandler::findFullPath(std::string path) {
	///static/file.html means you want the 7th character on
	//std::cerr << path.substr(7);
	//TODO: configure the home directory
	// std::cerr << "/usr/src/project/" + root_path_ + path.substr(7);
	// return "/usr/src/project/" + root_path_ + path.substr(7);
	// return "/usr/src/projects/bbk-simple-echo-server/" + root_path_ + path.substr(7);
	return "../" + root_path_ + path.substr(7);
	
}

void StaticHandler::setExtension(std::string path)
{
	int mIndex = path.find_last_of(".");
	std::string mExtension = path.substr(mIndex + 1);

	if (mExtension == "html") {
		ext = "text/html";
	} else if (mExtension == "jpg") {
		ext = "image/jpeg";
	} else if (mExtension == "zip") {
		ext = "application/zip";
	} else if (mExtension == "txt") {
		ext = "text/plain";
	}
}
#include "static_handler.h"
#include "request.h"
#include "response.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


StaticHandler::StaticHandler(Request *req, Response *resp, std::string root_dir)
	: RequestHandler(req, resp, root_dir)
{
	succeeded_ = process();
}


bool StaticHandler::process()
{
	if ( 
		req_->is_valid_syntax()
		&& req_->get_method() == "GET"
		&& req_->get_version() == "HTTP/1.1"
	) {
		build_response();
		return true;
	}
	else resp_->invalidate();
	
	return false;
}

bool StaticHandler::build_response()
{
	std::string path = req_->get_path();
	setExtension(path);

	resp_->set_version(req_->get_version());
	resp_->set_status(200);
	std::pair<std::string, std::string> header = std::make_pair("Content-type", ext);
	resp_->add_header(header);
	
	std::string full_path = findFullPath(path);
	std::ifstream f(full_path.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
	if (f.is_open()) {
		std::streampos mSize = f.tellg();
		char* mBuffer = new char[mSize];
		f.seekg(0, std::ios::beg);
		f.read(mBuffer, mSize);
		f.close();

		resp_->set_version(req_->get_version());
		resp_->set_status(200);
		std::pair<std::string, std::string> header = std::make_pair("Content-type", ext);
		resp_->add_header(header);
		resp_->set_body(std::string(mBuffer, mSize));
		delete mBuffer;
		return true;
	} else {
		resp_->set_version(req_->get_version());
		resp_->set_status(404);
		std::pair<std::string, std::string> header = std::make_pair("Content-type", "text/html");
		resp_->add_header(header);
		resp_->set_body("404 Not Found");
		std::cerr << "Could not open file.";
		return false;
	}
	return false;
}

std::string StaticHandler::findFullPath(std::string path) {
	///static/file.html means you want the 7th character on
	//std::cerr << path.substr(7);
	//TODO: configure the home directory
	return "/usr/src/projects/bbk-simple-echo-server/" + root_dir_ + path.substr(7);
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

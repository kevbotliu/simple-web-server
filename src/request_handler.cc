#include "request_handler.h"
#include <iostream>

RequestHandler::RequestHandler(Request *req, Response *resp)
	: succeeded_(true)
{
	if (!process(req, resp)) succeeded_ = false;
}

// (Mainly) Add conditions to this method to conform to spec details
bool RequestHandler::process(Request *req, Response *resp)
{
	if ( 
		req->is_valid_syntax()
		&& req->get_method() == "GET"
		&& req->get_version() == "HTTP/1.1"
	) {
		generate_resp(req, resp);
		return true;
	}
	else resp->invalidate();
	
	return false;
}

void RequestHandler::generate_resp(Request *req, Response *resp)
{
	resp->set_version(req->get_version());
	resp->set_status(200);
	std::pair<std::string, std::string> header = std::make_pair("Content-type", "text/plain");
	resp->add_header(header);
	resp->set_body(req->get_raw());
}

bool RequestHandler::succeeded() {return succeeded_;}
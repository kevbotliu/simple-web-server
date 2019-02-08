#include "static_handler.h"


StaticHandler::StaticHandler(Request *req, Response *resp)
	: RequestHandler(req, resp)
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

void StaticHandler::build_response()
{
	resp_->set_version(req_->get_version());
	resp_->set_status(200);
	std::pair<std::string, std::string> header = std::make_pair("Content-type", "text/plain");
	resp_->add_header(header);
	resp_->set_body(req_->get_raw());
}
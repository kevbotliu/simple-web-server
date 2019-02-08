#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "request.h"
#include "response.h"

class RequestHandler
{
public:
	RequestHandler(Request *req, Response *resp)
		: req_(req), resp_(resp), root_dir_(".") {}
	RequestHandler(Request *req, Response *resp, std::string root)
		: req_(req), resp_(resp), root_dir_(root) {}
	
	virtual bool process() = 0;
	virtual bool build_response() = 0;

	bool succeeded() {return succeeded_;}
protected:
	Request *req_;
	Response *resp_;
	bool succeeded_;
	std::string root_dir_;
};
#endif
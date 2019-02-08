#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "request.h"
#include "response.h"

class RequestHandler
{
public:
	RequestHandler(Request *req, Response *resp)
		: req_(req), resp_(resp) {}
	
	virtual bool process() = 0;
	virtual void build_response() = 0;

	bool succeeded() {return succeeded_;}
protected:
	Request *req_;
	Response *resp_;
	bool succeeded_;
};
#endif
#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include "request_handler.h"

class StaticHandler : public RequestHandler
{
public:
	StaticHandler(Request *req, Response *resp);

	bool process() override;
	void build_response() override;

};
#endif
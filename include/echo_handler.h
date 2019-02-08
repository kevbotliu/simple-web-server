#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "request_handler.h"

class EchoHandler : public RequestHandler
{
public:
	EchoHandler(Request *req, Response *resp);

	bool process() override;
	bool build_response() override;

};
#endif
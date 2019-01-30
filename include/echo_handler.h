#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "request.h"
#include "response.h"

class EchoHandler
{
public:
	EchoHandler(Request *req, Response *resp);
	bool succeeded();

private:
	bool process(Request *req, Response *resp);
	void generate_resp(Request *req, Response *resp);
	bool succeeded_;
};
#endif
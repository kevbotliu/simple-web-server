#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include "request_handler.h"

class StaticHandler : public RequestHandler
{
public:
	StaticHandler(Request *req, Response *resp);

	bool process() override;
	bool build_response() override;

private:
	std::string ext;
	
	void setExtension(std::string path);
	std::string findFullPath(std::string path);
};
#endif

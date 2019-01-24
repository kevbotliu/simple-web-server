#ifndef SESSION_TEST_H
#define SESSION_TEST_H

class session_test
{
public:
	session_test() {}
	std::string handle_read(const std::string request, size_t bytes_transferred);
};
#endif
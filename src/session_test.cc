#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>
#include "session_test.h"


std::string session_test::handle_read(const std::string request,
      size_t bytes_transferred)
  {
    std::string body = request;
    body = body.substr(0, bytes_transferred);
    std::string response = "";
    std::vector<std::string> lines;
    boost::split(lines, body, boost::is_any_of("\r\n"));
    if (lines.size() >= 1) {
        std::vector<std::string> statuses;
        boost::split(statuses, lines[0], boost::is_any_of(" "));

        if (statuses.size() == 3 && boost::iequals(statuses[0], "GET") && boost::iequals(statuses[2], "HTTP/1.1")) {
            std::string status_line = "HTTP/1.1 200 OK\r\n";
            std::string content_type = "Content-Type: text/plain\r\n";
            std::string headers = status_line + content_type + "\r\n";

            response = headers + body;
        }
    }
    return response;
  }
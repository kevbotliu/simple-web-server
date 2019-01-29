#include "session.h"
#include "request.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>

session::session(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

tcp::socket& session::socket()
  {
    return socket_;
  }

void session::start()
  {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        boost::bind(&session::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

void session::handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    { 
      std::string s = data_;
      Request *r = new Request(s.substr(0, bytes_transferred));



      // std::string body = data_;
      // body = body.substr(0, bytes_transferred);
      // std::string response = "";
      // std::vector<std::string> lines;
      // boost::split(lines, body, boost::is_any_of("\r\n"));
      // if (lines.size() >= 1) {
      //   std::vector<std::string> statuses;
      //   boost::split(statuses, lines[0], boost::is_any_of(" "));

      //   if (statuses.size() == 3 && boost::iequals(statuses[0], "GET") && boost::iequals(statuses[2], "HTTP/1.1")) {
      //     std::string status_line = "HTTP/1.1 200 OK\r\n";
      //     std::string content_type = "Content-Type: text/plain\r\n";
      //     std::string headers = status_line + content_type + "\r\n";

      //     response = headers + body;
      //   }
      // }

      std::string response = "";

      boost::asio::async_write(socket_,
          boost::asio::buffer(response, response.size()),
          boost::bind(&session::handle_write, this,
            boost::asio::placeholders::error));
    }
    else
    {
      delete this;
    }
  }

void session::handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_read_some(boost::asio::buffer(data_, max_length),
          boost::bind(&session::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      delete this;
    }
  }

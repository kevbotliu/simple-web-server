#include "session.h"
#include "request.h"
#include "response.h"
#include "echo_handler.h"
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
      Request *req = new Request(s.substr(0, bytes_transferred-1));
      Response *resp = new Response();

      EchoHandler handler(req, resp);

      boost::asio::async_write(socket_,
          boost::asio::buffer(resp->to_string(), resp->size()),
          boost::bind(&session::handle_write, this,
            boost::asio::placeholders::error));

      delete req;
      delete resp;
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

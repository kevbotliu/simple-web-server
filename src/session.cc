#include "session.h"
#include "request_handler.h"
#include "echo_handler.h"
#include "static_handler.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>

session::session(boost::asio::io_service& io_service, HandlerConfig* config)
    : socket_(io_service),
      config_(config),
      log()
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
      Request *req = new Request(s.substr(0, bytes_transferred));
      Response *resp = new Response();

      bool success = run_handler(req, resp);
      std::string output;

      if (success) output = resp->to_string();
      else output = "";

      boost::asio::async_write(socket_,
          boost::asio::buffer(output, output.length()),
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
    delete this;
    // if (!error)
    // {
    //   socket_.async_read_some(boost::asio::buffer(data_, max_length),
    //       boost::bind(&session::handle_read, this,
    //         boost::asio::placeholders::error,
    //         boost::asio::placeholders::bytes_transferred));
    // }
    // else
    // {
    //   delete this;
    // }
  }

bool session::run_handler(Request *req, Response *resp) {
  std::string req_path = req->get_path();

  std::string ip_addr = socket().remote_endpoint().address().to_string();

  for (Handler handler : config_->handlers_) {
    for (std::string path : handler.paths) {

      if (req_path.substr(0, path.size()) == path) {
        std::string message = "Session: IP: " + ip_addr + ": Entered " + handler.name + ".";
        log.log(message, boost::log::trivial::info);

        // REQUEST HANDLERS
        if (handler.name == "EchoHandler") {
          EchoHandler req_handler(req, resp);
          return req_handler.succeeded();
        }
        if (handler.name == "StaticHandler") {
          StaticHandler req_handler(req, resp, handler.root_dir);
          return req_handler.succeeded();
        }
        
      }
    }
  }
  std::string message = "Session: IP: " + ip_addr + ": Entered Default Handler (EchoHandler).";
  log.log(message, boost::log::trivial::info);

  EchoHandler handler(req, resp);
  return handler.succeeded();

}

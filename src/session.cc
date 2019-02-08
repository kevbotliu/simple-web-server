#include "session.h"
#include "request_handler.h"
#include "echo_handler.h"
#include "static_handler.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>

session::session(boost::asio::io_service& io_service, std::map<std::string, std::vector<std::string>> conf_paths)
    : socket_(io_service),
      conf_paths_(conf_paths),
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
          boost::asio::buffer(output, output.size()),
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

bool session::run_handler(Request *req, Response *resp) {
  std::string req_path = req->get_path();

  // std::cout << conf_paths_.count("echo") << ' ' << conf_paths_.count("static") << '\n';
  std::string ip_addr = socket().remote_endpoint().address().to_string();

  if (conf_paths_.count("echo")) {
    for (std::string conf_path : conf_paths_["echo"]) {
      if (req_path.size() >= conf_path.size() &&
          req_path.substr(0, conf_path.size()) == conf_path) {

        std::string message = "Session: IP: " + ip_addr + ": Entered EchoHandler.";
        log.log(message, boost::log::trivial::info);

        EchoHandler handler(req, resp);
        return handler.succeeded();
      }
    }
  }

  if (conf_paths_.count("static")) {
    for (std::string conf_path : conf_paths_["static"]) {
      if (req_path.size() >= conf_path.size() &&
          req_path.substr(0, conf_path.size()) == conf_path) {

        std::string message = "Session: IP: " + ip_addr + ": Entered StaticHandler.";
        log.log(message, boost::log::trivial::info);
          
        StaticHandler handler(req, resp);
        return handler.succeeded();
      }
    }
  }
  std::string message = "Session: IP: " + ip_addr + ": Entered Default Handler (EchoHandler).";
  log.log(message, boost::log::trivial::info);

  EchoHandler handler(req, resp);
  return handler.succeeded();

}

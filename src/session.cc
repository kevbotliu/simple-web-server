#include "session.h"
#include "request_handler.h"
#include "echo_handler.h"
#include "static_handler.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>

session::session(boost::asio::io_service& io_service, NginxConfig *config)
    : socket_(io_service),
      config_(config)
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
  std::string req_path, conf_path;
  for (const auto& statement : config_->statements_) {
    if (statement->tokens_[0] == "path" && 
        statement->tokens_[2] == "StaticHandler" &&
        statement->tokens_.size() == 3) {

      req_path = req->get_path();
      conf_path = statement->tokens_[1];
      if (req_path.size() >= conf_path.size() &&
          req_path.substr(0, conf_path.size()) == conf_path) {

        std::cout << "Entered StaticHandler\n";
          
        StaticHandler handler(req, resp);
        return handler.succeeded();
      }
    }
    
    if (statement->tokens_[0] == "path" && 
        statement->tokens_[2] == "EchoHandler" &&
        statement->tokens_.size() == 3) {

      req_path = req->get_path();
      conf_path = statement->tokens_[1];
      if (req_path.size() >= conf_path.size() &&
          req_path.substr(0, conf_path.size()) == conf_path) {

        std::cout << "Entered EchoHandler\n";

        EchoHandler handler(req, resp);
        return handler.succeeded();
      }
    }
  }

  std::cout << "Entered Default Handler (EchoHandler)\n";

  EchoHandler handler(req, resp);
  return handler.succeeded();

}

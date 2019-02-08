#include "server.h"

server::server(boost::asio::io_service& io_service, short port, NginxConfig *config)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      config_(config)
  {
    init_handlers();
    start_accept();
  }

void server::init_handlers() {
  std::vector<std::string> static_paths, echo_paths;
  for (const auto& statement : config_->statements_) {
    if (statement->tokens_[0] == "path" && 
        statement->tokens_[2] == "StaticHandler" &&
        statement->tokens_.size() == 3) {

      static_paths.push_back(statement->tokens_[1]);
    }
    
    if (statement->tokens_[0] == "path" && 
        statement->tokens_[2] == "EchoHandler" &&
        statement->tokens_.size() == 3) {

      echo_paths.push_back(statement->tokens_[1]);
    }
  }
  if (!echo_paths.empty()) conf_paths_["echo"] = echo_paths;
  if (!static_paths.empty()) conf_paths_["static"] = static_paths;
}

void server::start_accept()
  {
    session* new_session = new session(io_service_, conf_paths_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }

void server::handle_accept(session* new_session,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_session->start();
    }
    else
    {
      delete new_session;
    }

    start_accept();
  }

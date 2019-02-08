#include "server.h"
#include "logger.h"

server::server(boost::asio::io_service& io_service, short port, NginxConfig *config)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      config_(config),
      log()
  {
    init_handlers();
    start_accept();
  }

void server::init_handlers() {
  log.log("Server: Initializing Handlers...", boost::log::trivial::info);
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
    log.log("Server: Starting Acceptor...", boost::log::trivial::info);
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
      std::string ip_addr = new_session->socket().remote_endpoint().address().to_string();
      std::string message = "Session: IP: " + ip_addr + ": Starting A New Session...";
      log.log(message, boost::log::trivial::info);
      new_session->start();
    }
    else
    {
      delete new_session;
    }

    start_accept();
  }

#include "server.h"

#include "handler_config_builder.h"

server::server(boost::asio::io_service& io_service, short port, std::string config_file)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      log()
  {
    init_config(config_file);
    start_accept();
  }

void server::init_config(std::string config_file) {
  config_ = HandlerConfigBuilder()
    .set_config(config_file)
    .register_handler("EchoHandler")
    .register_handler("StaticHandler")
    .build();
  if (!config_->extract()) exit(1);
}

void server::start_accept()
  {
    log.log("Server: Starting Acceptor...", boost::log::trivial::info);
    session* new_session = new session(io_service_, config_);
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

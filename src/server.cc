#include "server.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

server::server(boost::asio::io_service& io_service, NginxConfig& config, short port)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      log()
  {
    config.extract();
    dispatcher_ = new Dispatcher(config);
    start_accept();
  }

server::~server() { delete dispatcher_; }

void server::start_accept()
  {
    log.log("Server: Starting Acceptor...", boost::log::trivial::info);
    session* new_session = new session(io_service_, dispatcher_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }

void server::handle_accept(session* new_session, const boost::system::error_code& error)
  {
    if (!error) {
      std::string ip_addr = new_session->socket().remote_endpoint().address().to_string();
      std::string message = "Session: IP: " + ip_addr + ": Starting A New Session...";
      log.log(message, boost::log::trivial::info);
      new_session->start();
    }

    start_accept();
  }

void server::shutdown()
  {
    log.log("Main: Server Shut Down.", boost::log::trivial::info);
    io_service_.stop();
    
    exit(0);
  }
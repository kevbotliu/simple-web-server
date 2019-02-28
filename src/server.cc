#include "server.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

server::server(const NginxConfig& config)
    : config_(config),
      signals_(io_service_),
      acceptor_(io_service_),
      dispatcher_(new Dispatcher(config)),
      log()
  {
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(boost::bind(&server::shutdown, this));

    tcp::resolver resolver(io_service_);
    tcp::resolver::query query("0.0.0.0", std::to_string(config_.port));
    tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();
  }

server::~server() { delete dispatcher_; }

void server::run()
  {
    std::vector<boost::shared_ptr<boost::thread> > threads;
    for (int i = 0; i < config_.num_threads; i++) {
      boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
      threads.push_back(thread);
    }

    for (int i = 0; i < threads.size(); i++) threads[i]->join();
  }

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
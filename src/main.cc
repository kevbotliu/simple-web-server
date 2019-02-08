#include "server.h"
#include "session.h"
#include "logger.h"
#include <cstdlib>
#include <iostream>

void shutdown_handler(
    const boost::system::error_code& error,
    int signal_number)
{
  if (!error)
  {
    logger log; 
    log.log("Main: Server Shut Down.", boost::log::trivial::error);
    exit(0);
  }
}

int main(int argc, char* argv[])
{
  logger log; 
  log.init_logging("main.log");

  try
  {
    if (argc != 2)
    {
      log.log("Main: Usage: server <config_file>", boost::log::trivial::error);
      return 1;
    }

    char* config_file = argv[1];
    // Validate nginx config file
    NginxConfigParser parser;
    NginxConfig config_out;
    if (!parser.Parse(config_file, &config_out)){
      log.log("Main: Missing or malformed configuration file detected", boost::log::trivial::error);
      return 1;
    }

    std::string port_number = std::to_string(parser.getPort());
    log.log("Main: Configuration Parsed Successfully.", boost::log::trivial::info);
    std::string message = "Main: Setting up Server on Port " + port_number + "...";
    log.log(message, boost::log::trivial::info);

    boost::asio::io_service io_service;
    log.log("Main: Starting Server...", boost::log::trivial::info);
    server s(io_service, parser.getPort(), &config_out);

    // Construct a signal set registered for process termination.
    boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
    signals.async_wait(shutdown_handler);
    io_service.run();

    // Start an asynchronous wait for one of the signals to occur.

  }
  catch (std::exception& e)
  {
    std::string error_message = "Main: Exception: ";
    error_message += e.what();
    log.log(error_message, boost::log::trivial::error);
  }

  return 0;
}

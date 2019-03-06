#include "server.h"
#include "session.h"
#include "logger.h"
#include <cstdlib>
#include <iostream>
#include <string>

#include "handler_factory.h"

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

    log.log("Main: Starting Server...", boost::log::trivial::info);

    NginxConfigParser parser;
    NginxConfig config;
    if (!parser.Parse(argv[1], &config)){
      log.log("Main: Missing or malformed configuration file detected", boost::log::trivial::error);
      return false;
    }
    config.extractHandlerInfo();

    log.log("Main: Configuration Parsed Successfully.", boost::log::trivial::info);
    std::string message = "Main: Setting up Server on Port " + std::to_string(config.port) + "...";
    log.log(message, boost::log::trivial::info);

    server s(config);
    s.run();

  }
  catch (std::exception& e)
  {
    std::string error_message = "Main: Exception: ";
    error_message += e.what();
    log.log(error_message, boost::log::trivial::error);
  }

  return 0;
}

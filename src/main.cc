#include "server.h"
#include "session.h"
#include "logger.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
  logger server_log; 

  try
  {
    if (argc != 2)
    {
      server_log.trivial_logging("Server Log: Usage: server <config_file>");
      return 1;
    }

    char* config_file = argv[1];
    // Validate nginx config file
    NginxConfigParser parser;
    NginxConfig config_out;
    if (!parser.Parse(config_file, &config_out)){
      server_log.trivial_logging("Server Log: Missing or malformed configuration file detected");
      return 1;
    }

    // printf("%d\n", parser.getPort());

    boost::asio::io_service io_service;
    server s(io_service, parser.getPort(), &config_out);

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::string error_message = "Server Log: Exception: ";
    error_message += e.what();
    server_log.trivial_logging(error_message);
  }

  return 0;
}

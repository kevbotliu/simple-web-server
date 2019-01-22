#include "server.h"
#include "session.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: server <config_file>\n";
      return 1;
    }

    char* config_file = argv[1];
    // Validate nginx config file
    NginxConfigParser parser;
    NginxConfig config_out;
    if (!parser.Parse(config_file, &config_out)){
      std::cerr << "Missing or malformed configuration file detected\n";
      return 1;
    }

    printf("%d\n", parser.getPort());

    boost::asio::io_service io_service;

    using namespace std; // For atoi.
    server s(io_service, parser.getPort());

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

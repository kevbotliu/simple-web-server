#include "server.h"
#include "session.h"
#include <cstdlib>
#include <iostream>

// Boost libraries
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>

void init_logging() {
  // Enable logging to file
  auto s = boost::log::add_file_log(
    // File name
    boost::log::keywords::file_name = "main.log",
    // Flush (write to file) immediately after error message
    boost::log::keywords::auto_flush = true
    // Midnight rotation
    boost::log::keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
    // 10MB rotation
    boost::log::keywords::rotationIsze = 10 * 1024 * 1024,
  );
  // Enable logging to console
  boost::log::add_console_log(); 

  boost::log::add_common_attributes();

}

int main(int argc, char* argv[])
{
  init_logging();

    BOOST_LOG_TRIVIAL(trace) << "This is a trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "This is a debug severity message";
    BOOST_LOG_TRIVIAL(info) << "This is an informational severity message"; 
    BOOST_LOG_TRIVIAL(warning) << "This is a warning severity message";
    BOOST_LOG_TRIVIAL(error) << "This is an error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "and this is a fatal severity message";

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

    // printf("%d\n", parser.getPort());

    boost::asio::io_service io_service;
    server s(io_service, parser.getPort());

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

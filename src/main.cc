#include "server.h"
#include "session.h"
#include <cstdlib>
#include <iostream>

// Boost libraries
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/thread.hpp>

void init_logging() {
  // Add common attributes for formatting purposes
  boost::log::add_common_attributes();

  // Enable logging to file
  auto s = boost::log::add_file_log(
    // File name
    boost::log::keywords::file_name = "main.log",
    // Flush (write to file) immediately after error message
    boost::log::keywords::auto_flush = true,
    // Midnight rotation
    boost::log::keywords::time_based_rotation = 
      boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
    // 10MB rotation
    boost::log::keywords::rotation_size = 10 * 1024 * 1024,

    // Format
    // boost::log::keywords::format = "%Message%"
    boost::log::keywords::format = (
      boost::log::expressions::stream << "[" << 
      boost::log::expressions::format_date_time<boost::posix_time::ptime>
        ("TimeStamp", "%Y-%m-%d %H:%M:%S")
        << "]: " << boost::this_thread::get_id() << " "
        << boost::log::trivial::severity << ": "
        << boost::log::expressions::smessage
    )
  );
  // Enable logging to console
  boost::log::add_console_log(); 
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

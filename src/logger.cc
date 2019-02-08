#include "logger.h"

logger::logger() {
  init_logging();
}

void logger::init_logging() {
  // Add common attributes for formatting purposes
  boost::log::add_common_attributes();

  // Enable logging to file
  auto s = boost::log::add_file_log(
    // File name
    keywords::file_name = "main.log",
    // Flush (write to file) immediately after error message
    keywords::auto_flush = true,
    // Midnight rotation
    keywords::time_based_rotation = 
      boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
    // 10MB rotation
    keywords::rotation_size = 10 * 1024 * 1024,

    // Format
    keywords::format = (
      expressions::stream << "[" << 
      expressions::format_date_time<boost::posix_time::ptime>
        ("TimeStamp", "%Y-%m-%d %H:%M:%S")
        << "]: " << boost::this_thread::get_id() << " "
        << boost::log::trivial::severity << ": "
        << expressions::smessage
    )
  );

  // Enable logging to console
  boost::log::add_console_log(); 
}

void logger::trivial_logging(std::__cxx11::string error_message) {
    BOOST_LOG_TRIVIAL(error)  << error_message;
}
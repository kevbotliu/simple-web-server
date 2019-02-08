#include "logger.h"

logger::logger() {}

void logger::init_logging(std::string log_name) {
  // Add common attributes for formatting purposes
  boost::log::add_common_attributes();

  // Enable logging to file
  auto s = boost::log::add_file_log(
    // File name
    keywords::file_name = log_name,
    // Flush (write to file) immediately after error message
    keywords::auto_flush = true,
    // Midnight rotation
    keywords::time_based_rotation = 
      boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
    // 10MB rotation
    keywords::rotation_size = 10 * 1024 * 1024,
    keywords::open_mode = std::ios_base::app,

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

void logger::log(std::__cxx11::string message, boost::log::trivial::severity_level sev_lvl) {
    switch (sev_lvl){
        case boost::log::trivial::trace:
            BOOST_LOG_TRIVIAL(trace) << message;
            break;
        case boost::log::trivial::debug:
            BOOST_LOG_TRIVIAL(debug) << message;
            break;
        case boost::log::trivial::info:
            BOOST_LOG_TRIVIAL(info) << message;
            break;
        case boost::log::trivial::warning:
            BOOST_LOG_TRIVIAL(warning) << message;
            break;
        case boost::log::trivial::error:
            BOOST_LOG_TRIVIAL(error) << message;
            break;
        case boost::log::trivial::fatal:
            BOOST_LOG_TRIVIAL(fatal) << message;
            break;
    }
}
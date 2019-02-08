#ifndef LOGGER_H
#define LOGGER_H

// Boost libraries
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/thread.hpp>

namespace keywords = boost::log::keywords;
namespace expressions = boost::log::expressions;

class logger
{
  public:
    logger();
    void log(std::__cxx11::string message, boost::log::trivial::severity_level sev_lvl);
    void init_logging(std::string log_name);
  private:
};
#endif

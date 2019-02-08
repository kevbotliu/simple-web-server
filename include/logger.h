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
#include <boost/thread.hpp>

namespace keywords = boost::log::keywords;
namespace expressions = boost::log::expressions;

class logger
{
  public:
    logger();
    void trivial_logging(std::__cxx11::string error_message);
  private:
    void init_logging();
};
#endif

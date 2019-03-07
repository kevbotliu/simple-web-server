#ifndef SHARED_H
#define SHARED_H

#include <string>
#include <vector>
#include <utility>
#include <boost/thread.hpp>

class RequestHistory {
  public:
  	RequestHistory() {}
  	std::vector<std::pair<std::string, int>> get_request_history();
  	void update_request_history(std::pair<std::string, int>);
  private:
    static std::vector<std::pair<std::string, int>> history;
    boost::mutex mutex;
};

#endif
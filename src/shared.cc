#include "shared.h"

std::vector<std::pair<std::string, int>> RequestHistory::get_request_history() {
	boost::mutex::scoped_lock l(mutex);
	return history;
}
  
void RequestHistory::update_request_history(std::pair<std::string, int> req) {
	boost::mutex::scoped_lock l(mutex);
	history.push_back(req);
}

std::vector<std::pair<std::string, int>> RequestHistory::history;
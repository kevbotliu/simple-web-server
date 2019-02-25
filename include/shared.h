#ifndef SHARED_H
#define SHARED_H

#include <string>
#include <vector>
#include <utility>

class RequestHistory {
  public:
    static std::vector<std::pair<std::string, int>> history;
};

#endif
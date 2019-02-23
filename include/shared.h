#ifndef SHARED_H
#define SHARED_H

#include <string>
#include <vector>
#include <utility>

typedef struct {
	std::string name;
	std::string path;
	std::string root_path;
  std::string remote_url; // TODO: might want to delete
  std::string remote_port;
} HandlerBlock;

class HandlerInfo {
  public:
    static std::vector<HandlerBlock> handler_blocks;
};

class RequestHistory {
  public:
    static std::vector<std::pair<std::string, int>> history;
};

#endif
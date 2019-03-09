#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <iostream>
#include <string.h>

class Pathfinder {

public:
	Pathfinder(std::string path)  : path_(path) {}
	std::string find_full_path(std::string server_root_path, std::string block_root_path);
	std::string get_extension() { return ext_; }
	void set_extension();
private:
	std::string path_;
	std::string ext_;
};

#endif

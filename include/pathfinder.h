#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <iostream>
#include <string.h>

class Pathfinder {

public:
	Pathfinder(std::string path);
	std::string findFullPath(std::string path, std::string root_path_);
private:
	std::string ext;
	void setExtension(std::string path);
};

#endif

#include "pathfinder.h"

Pathfinder::Pathfinder(std::string path) {
	setExtension(path);
}

std::string Pathfinder::findFullPath(std::string path, std::string root_path_) {
	///static/file.html means you want the 7th character on
	//std::cerr << path.substr(7);
	//TODO: configure the home directory
	// std::cerr << "/usr/src/project/" + root_path_ + path.substr(7);
	// return "/usr/src/project/" + root_path_ + path.substr(7);
	// return "/usr/src/projects/bbk-simple-echo-server/" + root_path_ + path.substr(7);
	return "../" + root_path_ + path.substr(7);
}

void Pathfinder::setExtension(std::string path)
{
	int mIndex = path.find_last_of(".");
	std::string mExtension = path.substr(mIndex + 1);

	if (mExtension == "html") {
		ext = "text/html";
	} else if (mExtension == "jpg") {
		ext = "image/jpeg";
	} else if (mExtension == "zip") {
		ext = "application/zip";
	} else if (mExtension == "txt") {
		ext = "text/plain";
	}
}

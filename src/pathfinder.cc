#include "pathfinder.h"

std::string Pathfinder::find_full_path(std::string server_root_path, std::string block_root_path) {
	///static/file.html means you want the 7th character on
	//std::cerr << path.substr(7);
	//TODO: configure the home directory
	// std::cerr << "/usr/src/project/" + root_path_ + path.substr(7);
	// return "/usr/src/project/" + root_path_ + path.substr(7);
	// return "/usr/src/projects/bbk-simple-echo-server/" + root_path_ + path.substr(7);
	return server_root_path + block_root_path + path_.substr(7);
}

void Pathfinder::set_extension() {
	ext_ = "";
	int mIndex = path_.find_last_of(".");
	std::string mExtension = path_.substr(mIndex + 1);

	if (mExtension == "html") {
		ext_ = "text/html";
	} else if (mExtension == "jpg") {
		ext_ = "image/jpeg";
	} else if (mExtension == "zip") {
		ext_ = "application/zip";
	} else if (mExtension == "txt") {
		ext_ = "text/plain";
	}
}

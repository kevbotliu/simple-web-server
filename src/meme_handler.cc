#include "meme_handler.h"
#include <numeric>
#include <streambuf>
#include <fstream>
#include <string>
#include "logger.h"


RequestHandler* MemeHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new MemeHandler(config, root_path);
}

std::unique_ptr<Reply> MemeHandler::HandleRequest(const Request& request) {	
	std::string subpath = request.get_path().erase(0, 5);

	if (request.get_method() == "GET") {
		// Personally I believe this should be the default
		if (subpath.find("/new") == 0) {
			return handleNew();
		}
		if (subpath.find("/view") == 0) {
			if (subpath.size() > 9 && subpath.find("?id=") == 5) {
				std::string id_string = subpath.substr(9);
				int id = atoi(id_string.c_str());
				if (id) return handleView(id);
			}
		}
		if (subpath.find("/list") == 0) {
			return handleList();
		}
	}

	if (request.get_method() == "POST") {
		if (subpath.find("/create") == 0) {
			// Do this...
		}
	}

	return std::unique_ptr<Reply>(new Reply(false));
}

// Handle creating memes
std::unique_ptr<Reply> MemeHandler::handleNew() {
	
	std::string filepath = "../" + root_path_ + "/" + "memeform.html";

	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));

	// Credit: https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
	std::ifstream t(filepath);
	std::stringstream buffer;
	buffer << t.rdbuf();
	args.body = buffer.str();

	return std::unique_ptr<Reply>(new Reply(args));

}
// Handle viewing individual memes.
std::unique_ptr<Reply> MemeHandler::handleView(int id) {
	NginxConfig meme_info;
	NginxConfigParser parser;
	std::string filepath = "../" + root_path_ + "/" + "saved_memes";

	mutex.lock();
	parser.Parse(filepath.c_str(), &meme_info);
	mutex.unlock();

	std::string image_path = "";
	std::string top_text = "";
	std::string bottom_text = "";
	for (auto statement : meme_info.statements_) {
		if (std::stoi(statement->tokens_[0]) == id) {
			for (auto child_statement : statement->child_block_->statements_) {
				if (child_statement->tokens_.size() == 2 &&
					child_statement->tokens_[0] == "image") {
					image_path = child_statement->tokens_[1];
				}
				if (child_statement->tokens_.size() >= 2 &&
					child_statement->tokens_[0] == "top") {
					bool first = true;
					for (const auto &piece : child_statement->tokens_) {
						if (first) {
							first = false;
							continue;
						}
						top_text += piece + " ";
					}
					top_text = top_text.substr(0, top_text.length()-1);
				}
				if (child_statement->tokens_.size() >= 2 &&
					child_statement->tokens_[0] == "bottom") {
					bool first = true;
					for (const auto &piece : child_statement->tokens_) {
						if (first) {
							first = false;
							continue;
						}
						bottom_text += piece + " ";
					}
					bottom_text = bottom_text.substr(0, bottom_text.length()-1);
				}
			}
		}
	}
	if (image_path == "" ||
		top_text == "" ||
		bottom_text == "") {
		return std::unique_ptr<Reply>(new Reply(false));
	}

	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));

	std::string page_link = "<head><link href=\"https://fonts.googleapis.com/css?family=Oswald\" rel=\"stylesheet\"></head>";
	std::string page_styles = "<style> body {display: flex; justify-content: center; position: relative;} span {color: black; font: 4em bold; text-transform: uppercase; font-family: 'Oswald', sans-serif; sans-serif; position: absolute; text-align: center; width: 100%;} #top {top: 0;} #bottom {bottom: 0;}</style>";
	std::string page_body = "<body>";
	// page_body += "<img src=\"https://www.google.com/images/branding/googlelogo/2x/googlelogo_color_272x92dp.png\">";
	page_body += "<img src=\"../static/" + image_path + "\">";
	page_body += "<span id=\"top\">" + top_text +"</span>";
	page_body += "<span id=\"bottom\">" + bottom_text + "</span></body>";

	args.body = page_link + page_styles + page_body;

	return std::unique_ptr<Reply>(new Reply(args));
}

std::unique_ptr<Reply> MemeHandler::handleList() {
	NginxConfig meme_info;
	NginxConfigParser parser;
	std::string filepath = "../" + root_path_ + "/" + "saved_memes";

	mutex.lock();
	parser.Parse(filepath.c_str(), &meme_info);
	mutex.unlock();

	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));

	std::string page_link = "<head><link href=\"https://fonts.googleapis.com/css?family=Oswald\" rel=\"stylesheet\"></head>";
	// std::string page_styles = "<style> body {display: flex; justify-content: center; position: relative;}</style>";
	std::string page_body = "<body>";

	for (auto statement : meme_info.statements_) {
		std::string id = statement->tokens_[0];
		page_body += "<li> <a href=\"/meme/view?id=" + id + "\"> " + id + " </a></li>";
	}

	args.body = page_link + page_body;

	return std::unique_ptr<Reply>(new Reply(args));
}

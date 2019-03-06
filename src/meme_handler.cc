#include "meme_handler.h"
#include <numeric>

RequestHandler* MemeHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new MemeHandler(config, root_path);
}

std::unique_ptr<Reply> MemeHandler::HandleRequest(const Request& request) {
	if (!request.is_valid()) return std::unique_ptr<Reply>(nullptr);
	if (request.get_method() != "GET") return std::unique_ptr<Reply>(nullptr);

	// Testing id
	// id will be received by the url parameter e.g. /meme/view?id=123456789
	return handleView(123456789);
}

std::unique_ptr<Reply> MemeHandler::handleView(int id) {
	NginxConfig meme_info;
	NginxConfigParser parser;
	std::string filepath = "../" + root_path_ + "/" + "saved_memes";
	parser.Parse(filepath.c_str(), &meme_info);

	std::string image_path, top_text, bottom_text;
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

	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));

	std::string page_link = "<head><link href=\"https://fonts.googleapis.com/css?family=Oswald\" rel=\"stylesheet\"></head>";
	std::string page_styles = "<style> body {display: flex; justify-content: center; position: relative;} span {color: white; font: 4em bold; text-transform: uppercase; font-family: 'Oswald', sans-serif; sans-serif; position: absolute; text-align: center; width: 100%;} #top {top: 0;} #bottom {bottom: 0;}</style>";
	std::string page_body = "<body>";
	// page_body += "<img src=\"https://www.google.com/images/branding/googlelogo/2x/googlelogo_color_272x92dp.png\">";
	page_body += "<img src=\"../" + root_path_ + "/" + image_path + "\">";
	page_body += "<span id=\"top\">" + top_text +"</span>";
	page_body += "<span id=\"bottom\">" + bottom_text + "</span></body>";

	args.body = page_link + page_styles + page_body;

	return std::unique_ptr<Reply>(new Reply(args));
}
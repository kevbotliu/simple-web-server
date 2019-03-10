#include "meme_handler.h"
#include <numeric>
#include <streambuf>
#include <fstream>
#include <string>
#include "logger.h"
#include <boost/algorithm/string.hpp>


RequestHandler* MemeHandler::create(const NginxConfig& config, const std::string& root_path) {
	return new MemeHandler(config, root_path);
}

ParamMap MemeHandler::extract_params(const Request& request) {
	ParamMap params;

	std::string param_str = "";
	std::string method = request.get_method();
	std::string path = request.get_path();
	if (method == "GET") {
		std::vector<std::string> tokens;
	    boost::split(tokens, path, boost::is_any_of("?"));

	    if (tokens.size() > 1) param_str = tokens[1];
	}
	if (method == "POST") {
		param_str = request.get_body();
	}

    std::vector<std::string> key_pairs;
	boost::split(key_pairs, param_str, boost::is_any_of("&"));

	for (auto key_pair : key_pairs) {
		auto ind = key_pair.find('=');
		if (ind != std::string::npos) {
			params.insert(std::make_pair(key_pair.substr(0, ind), key_pair.substr(ind+1)));
		}
	}

	return params;
}



std::unique_ptr<Reply> MemeHandler::HandleRequest(const Request& request) {	
	std::string subpath = request.get_path().erase(0, 5);
	ParamMap params = extract_params(request);

	if (request.get_method() == "GET") {

		if (subpath.empty() ||
			subpath == "/" ||
			subpath.find("/new") == 0) return handleNew();
		if (subpath.find("/view") == 0) return handleView(params);
		if (subpath.find("/list") == 0) return handleList(params);
	}

	if (request.get_method() == "POST") {
		if (subpath.find("/create") == 0) return handleCreate(params);
	}

	return std::unique_ptr<Reply>(new Reply(false));;
}

// Handle creating memes
std::unique_ptr<Reply> MemeHandler::handleNew() {
	
	std::string filepath = config_.server_root_path + root_path_ + "/" + "memeform.html";

	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));

	// Credit: https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
	mutex.lock();
	std::ifstream t(filepath);
	std::stringstream buffer;
	buffer << t.rdbuf();
	args.body = buffer.str();
	mutex.unlock();

	return std::unique_ptr<Reply>(new Reply(args));

}
// Handle viewing individual memes.
std::unique_ptr<Reply> MemeHandler::handleView(ParamMap& params) {
	// Param guard
	if (params.find("id") == params.end()) {
		return std::unique_ptr<Reply>(new Reply(false));
	}

	int id = atoi(params["id"].c_str());

	NginxConfig meme_info;
	NginxConfigParser parser;
	std::string filepath = config_.server_root_path + root_path_ + "/" + "saved_memes";

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
	std::string page_styles = "<style> body {display: flex; justify-content: center; align-items: center; flex-direction: column;} span {color: white; font: 3em bold; text-transform: uppercase; font-family: 'Oswald', sans-serif; position: absolute; text-align: center; width: 100%;} #top {left: 0; top: 0;} #bottom {left: 0; bottom: 0;} div {position: relative;} img {min-width: 300px; min-height: 300px;}</style>";
	std::string page_body = "<body>";
	// page_body += "<img src=\"https://www.google.com/images/branding/googlelogo/2x/googlelogo_color_272x92dp.png\">";
	page_body += "<div><img src=\"../static/" + image_path + "\">";
	page_body += "<span id=\"top\">" + top_text +"</span>";
	page_body += "<span id=\"bottom\">" + bottom_text + "</span></div></body>";

	args.body = page_link + page_styles + page_body;

	return std::unique_ptr<Reply>(new Reply(args));
}

std::unique_ptr<Reply> MemeHandler::handleList(ParamMap& params) {

	// Check search term
	// if (params.find("search") == params.end()) {
	// 	return std::unique_ptr<Reply>(new Reply(false));
	// }

	NginxConfig meme_info;
	NginxConfigParser parser;
	std::string filepath = config_.server_root_path + root_path_ + "/" + "saved_memes";

	mutex.lock();
	parser.Parse(filepath.c_str(), &meme_info);
	mutex.unlock();

	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));

	std::string page_link = "<head><link href=\"https://fonts.googleapis.com/css?family=Oswald\" rel=\"stylesheet\"></head>";
	// std::string page_styles = "<style> body {display: flex; justify-content: center; position: relative;}</style>";
	std::string page_body = "<body>";

	page_body += "<form action=\"/meme/list\" method=\"post\">";
	page_body += "<label>Search: </label>";
	page_body += "<input type=\"text\" name=\"term\">";
	page_body += "<input type=\"submit\" value=\"Search\">";
	page_body += "</form>";

	for (auto statement : meme_info.statements_) {
		std::string id = statement->tokens_[0];


		std::cout << statement->child_block_->statements_[0]->tokens_[0] << " " << statement->child_block_->statements_[1]->tokens_[0] << " " << statement->child_block_->statements_[2]->tokens_[0] << "\n";



		page_body += "<li> <a href=\"/meme/view?id=" + id + "\"> " + id + " </a></li>";
	}

	args.body = page_link + page_body;

	return std::unique_ptr<Reply>(new Reply(args));
}

std::unique_ptr<Reply> MemeHandler::handleCreate(ParamMap& params) {
	// Param guard
	if (params.find("memeselect") == params.end() ||
		params.find("top") == params.end() ||
		params.find("bot") == params.end()) {
		return std::unique_ptr<Reply>(new Reply(false));
	}

	std::string memeName = params["memeselect"];
	std::string topText = params["top"];
	std::string botText = params["bot"];

	// Write that information to the file
	NginxConfig meme_info;
	std::string filepath = config_.server_root_path + root_path_ + "/" + "saved_memes";

	srand(time(NULL));
	std::string saved_id = std::to_string(rand() % 1000000000);

	mutex.lock();
	std::ofstream t;
	t.open(filepath, std::fstream::app);
	if (t.is_open()) {
		t << "\n\n" << saved_id << " {\n\t";
		t << "image " << memeName << ";\n\t";
		t << "top " << topText << ";\n\t";
		t << "bottom " << botText << ";\n";
		t << "}\n";
	}
	mutex.unlock();

	// Display a HTML confirmation
	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));

	std::string page_link = "<head><link href=\"https://fonts.googleapis.com/css?family=Oswald\" rel=\"stylesheet\"></head>";
	std::string page_body = "<body><h1>Meme saved with id: ";
	page_body += "<a href=\"/meme/view?id=" + saved_id + "\">" + saved_id + "</a></h1></body>";

	args.body = page_link + page_body;

	return std::unique_ptr<Reply>(new Reply(args));
}

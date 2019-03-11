#include "meme_handler.h"
#include <numeric>
#include <streambuf>
#include <fstream>
#include <string>
#include <sqlite3.h>
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
		if (subpath.find("/edit") == 0) return handleEdit();
	}

	if (request.get_method() == "POST") {
		if (subpath.find("/create") == 0) return handleCreate(params);
		if (subpath.find("/update") == 0) return handleUpdate(params);
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

	// This website was a great help with SQL stuff: https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
	// And this is for stepwise SQLite: https://stackoverflow.com/questions/3957343/how-to-read-data-from-sqlite-database

	sqlite3 *meme_db;
	int rc;
	sqlite3_stmt* stmt;
	bool found = false;

	std::string image_path = "";
	std::string top_text = "";
	std::string bottom_text = "";
	std::string meme_id = params["id"];

	std::string filepath = config_.server_root_path + root_path_ + "/" + "saved_memes.db";

	mutex.lock();

	// Open SQL database
	rc = sqlite3_open(filepath.c_str(), &meme_db);
	if (rc) { // Failed opening
		mutex.unlock();
		return std::unique_ptr<Reply>(new Reply(false));
	}

	// Execute SQL statement for selection
	std::string sql_exec = "SELECT * from MEMES";
	if (sqlite3_prepare_v2(meme_db, sql_exec.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
		sqlite3_close(meme_db);
		sqlite3_finalize(stmt);
		mutex.unlock();
		return std::unique_ptr<Reply>(new Reply(false));		
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		if (sqlite3_column_int(stmt, 0) == id) {
			image_path = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
			top_text = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
			bottom_text = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
			found = true;
			break;
		}
	}

	if ( !(rc == SQLITE_DONE || rc == SQLITE_ROW) || found == false) {
		mutex.unlock();
		return std::unique_ptr<Reply>(new Reply(false));		
	}

	sqlite3_finalize(stmt);
	sqlite3_close(meme_db);

	mutex.unlock();


	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));

	std::string page_link = "<head><link href=\"https://fonts.googleapis.com/css?family=Oswald\" rel=\"stylesheet\"></head>";
	std::string page_styles = "<style> body {display: flex; justify-content: center; align-items: center; flex-direction: column;} span {color: white; font: 3em bold; text-transform: uppercase; font-family: 'Oswald', sans-serif; position: absolute; text-align: center; width: 100%;} #top {left: 0; top: 0;} #bottom {left: 0; bottom: 0;} div {position: relative;} img {min-width: 300px; min-height: 300px;}</style>";
	std::string page_body = "<body>";
	// page_body += "<img src=\"https://www.google.com/images/branding/googlelogo/2x/googlelogo_color_272x92dp.png\">";
	page_body += "<div><img src=\"../static/" + image_path + "\">";
	page_body += "<span id=\"top\">" + top_text +"</span>";
	page_body += "<span id=\"bottom\">" + bottom_text + "</span></div>";
	page_body += "<a href=\"/meme/edit?id=" + meme_id + "&top=" + top_text + "&bot=" + bottom_text + "\">edit</a></h1></body>";


	args.body = page_link + page_styles + page_body;

	return std::unique_ptr<Reply>(new Reply(args));
}

std::unique_ptr<Reply> MemeHandler::handleList(ParamMap& params) {

	// Check search term
	// if (params.find("search") == params.end()) {
	// 	return std::unique_ptr<Reply>(new Reply(false));
	// }

	std::string filepath = config_.server_root_path + root_path_ + "/" + "saved_memes.db";

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

	std::string page_list = "<ul>";

	sqlite3 *meme_db;
	int rc;
	sqlite3_stmt* stmt;

	mutex.lock();

	// Open SQL database
	rc = sqlite3_open(filepath.c_str(), &meme_db);
	if (rc) { // Failed opening
		mutex.unlock();
		return std::unique_ptr<Reply>(new Reply(false));
	}

	// Execute SQL statement for list
	std::string sql_exec = "SELECT * from MEMES";
	if (sqlite3_prepare_v2(meme_db, sql_exec.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
		sqlite3_close(meme_db);
		sqlite3_finalize(stmt);
		mutex.unlock();	
		return std::unique_ptr<Reply>(new Reply(false));	
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

		int id = sqlite3_column_int(stmt, 0);
		page_body += "<li> <a href=\"/meme/view?id=" + std::to_string(id) + "\"> " + std::to_string(id) + " </a></li>";

		std::cout << sqlite3_column_text(stmt, 1) << " " << sqlite3_column_text(stmt, 2) << " " << sqlite3_column_text(stmt, 3) << "\n";
	}

	if (rc != SQLITE_DONE) {
		mutex.unlock();
		return std::unique_ptr<Reply>(new Reply(false));		
	}

	sqlite3_finalize(stmt);
	sqlite3_close(meme_db);

	mutex.unlock();

	page_list += "</ul>";

	args.body = page_link + page_body + page_list;

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
	std::string filepath = config_.server_root_path + root_path_ + "/" + "saved_memes.db";

	srand(time(NULL));
	std::string saved_id = std::to_string(rand() % 1000000000);

	sqlite3 *meme_db;
	int rc;
	sqlite3_stmt* stmt;

	mutex.lock();

	// Open SQL database
	rc = sqlite3_open(filepath.c_str(), &meme_db);
	if (rc) { // Failed opening
		mutex.unlock();
		return std::unique_ptr<Reply>(new Reply(false));
	}

	// Execute SQL statement for list
	std::string sql_exec = "INSERT INTO MEMES VALUES ("
		+ saved_id + ", "
		+ '"' + memeName + '"' + ", "
		+ '"' + topText + '"' + ", "
		+ '"' + botText + '"' + ")";

	if (sqlite3_prepare_v2(meme_db, sql_exec.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
		sqlite3_close(meme_db);
		sqlite3_finalize(stmt);
		mutex.unlock();	
		return std::unique_ptr<Reply>(new Reply(false));	
	}

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE) {
		mutex.unlock();
		return std::unique_ptr<Reply>(new Reply(false));		
	}

	sqlite3_finalize(stmt);
	sqlite3_close(meme_db);

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

std::unique_ptr<Reply> MemeHandler::handleEdit() {
	
	std::string filepath = config_.server_root_path + root_path_ + "/" + "memeedit.html";

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

std::unique_ptr<Reply> MemeHandler::handleUpdate(ParamMap& params) {

	// Param guard
	if (params.find("memeselect") == params.end() ||
		params.find("top") == params.end() ||
		params.find("bot") == params.end() ||
		params.find("id") == params.end()) {
		return std::unique_ptr<Reply>(new Reply(false));
	}

	std::string memeName = params["memeselect"];
	std::string topText = params["top"];
	std::string botText = params["bot"];
	std::string memeID = params["id"];

	// Write that information to the file
	std::string filepath = config_.server_root_path + root_path_ + "/" + "saved_memes.db";

	sqlite3 *meme_db;
	int rc;
	sqlite3_stmt* stmt;

	mutex.lock();

	// Open SQL database
	rc = sqlite3_open(filepath.c_str(), &meme_db);
	if (rc) { // Failed opening
		mutex.unlock();
		return std::unique_ptr<Reply>(new Reply(false));
	}

	std::string sql_exec = "UPDATE memes SET top_text = \""
		+ botText + "\", "
		+ "bottom_text = \"" + topText + "\", "
		+ "image = \"" + memeName
		+ "\" WHERE meme_id = " + memeID;

	if (sqlite3_prepare_v2(meme_db, sql_exec.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
		sqlite3_close(meme_db);
		sqlite3_finalize(stmt);
		mutex.unlock();	
		return std::unique_ptr<Reply>(new Reply(false));
	}

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE) {
		mutex.unlock();
		return std::unique_ptr<Reply>(new Reply(false));		
	}

	sqlite3_finalize(stmt);
	sqlite3_close(meme_db);

	mutex.unlock();

	// Display a HTML confirmation
	ReplyArgs args;
	args.headers.push_back(std::make_pair("Content-type", "text/html"));

	std::string page_link = "<head><link href=\"https://fonts.googleapis.com/css?family=Oswald\" rel=\"stylesheet\"></head>";
	std::string page_body = "<body><h1>Meme updated with id: ";
	page_body += "<a href=\"/meme/view?id=" + memeID + "\">" + memeID + "</a></h1></body>";

	args.body = page_link + page_body;

	return std::unique_ptr<Reply>(new Reply(args));
}


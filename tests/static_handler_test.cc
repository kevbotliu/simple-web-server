#include "gtest/gtest.h"
#include "request.h"
#include "reply.h"
#include "static_handler.h"

class StaticHandlerTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() {}

    bool success;
};

TEST_F(StaticHandlerTest, ValidResponseTest) {
    success = true;
    std::string s = "GET /static/blah.txt HTTP/1.1\r\n\r\n";
    const Request *req = new Request(s);
    NginxConfig config;
    std::string root_path = "static_src";
    success = StaticHandler::create(config, root_path)->HandleRequest(*req)->is_valid();
    EXPECT_TRUE(success);
}

TEST_F(StaticHandlerTest, StatusCodeResponseTest) {
    success = true;
    std::string s = "GET /static/blah.txt HTTP/1.1\r\n\r\n";
    const Request *req = new Request(s);
    NginxConfig config;
    std::string root_path = "static_src";
    success = (StaticHandler::create(config, root_path)->HandleRequest(*req)->get_status_code()==200);
    EXPECT_TRUE(success);
}

TEST_F(StaticHandlerTest, NotFoundResponseTest) {
    success = true;
    std::string s = "GET /static/bla.txt HTTP/1.1\r\n\r\n";
    const Request *req = new Request(s);
    NginxConfig config;
    std::string root_path = "static_src";
    success = (StaticHandler::create(config, root_path)->HandleRequest(*req)->get_status_code()==404);
    EXPECT_TRUE(success);
}

TEST_F(StaticHandlerTest, CorrectResponseTest) {
    success = true;
    std::string s = "GET /static/blah.txt HTTP/1.1\r\n\r\n";
    const Request *req = new Request(s);
    NginxConfig config;
    std::string root_path = "static_src";
    success = (StaticHandler::create(config, root_path)->HandleRequest(*req)->get_body()=="words words words newline\nmore words\n2\nnewlines\nend");
    EXPECT_TRUE(success);
}

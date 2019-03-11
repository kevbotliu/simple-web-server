#include "gtest/gtest.h"
#include "request.h"
#include "reply.h"
#include "static_handler.h"
#include "config_parser.h"

class StaticHandlerTest : public ::testing::Test {
  protected:
    void SetUp() override {
        NginxConfigParser parser;
        assert(parser.Parse("../server_config_cloud", &config));
        config.extractHandlerInfo();
    }
    void TearDown() {}

    NginxConfig config;
    std::string root_path = "static_src";
    bool success;
};

TEST_F(StaticHandlerTest, ValidResponseTest) {
    std::string s = "GET /static/blah.txt HTTP/1.1\r\n\r\n";
    const Request *req = new Request(s);
    success = StaticHandler::create(config, root_path)->HandleRequest(*req)->is_valid();
    EXPECT_TRUE(success);
}

// TEST_F(StaticHandlerTest, StatusCodeResponseTest) {
//     std::string s = "GET /static/blah.txt HTTP/1.1\r\n\r\n";
//     const Request *req = new Request(s);
//     success = (StaticHandler::create(config, root_path)->HandleRequest(*req)->get_status_code()==200);
//     EXPECT_TRUE(success);
// }

TEST_F(StaticHandlerTest, NotFoundResponseTest) {
    std::string s = "GET /static/bla.txt HTTP/1.1\r\n\r\n";
    const Request *req = new Request(s);
    success = (StaticHandler::create(config, root_path)->HandleRequest(*req)->get_status_code()==404);
    EXPECT_TRUE(success);
}

// TEST_F(StaticHandlerTest, CorrectResponseTest) {
//     std::string s = "GET /static/blah.txt HTTP/1.1\r\n\r\n";
//     const Request *req = new Request(s);
//     success = (StaticHandler::create(config, root_path)->HandleRequest(*req)->get_body()=="words words words newline\nmore words\n2\nnewlines\nend");
//     EXPECT_TRUE(success);
// }

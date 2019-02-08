#include "gtest/gtest.h"
#include "echo_handler.h"
#include "request.h"
#include "response.h"

class EchoHandlerTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() {}

    bool success;
};

TEST_F(EchoHandlerTest, CorrectRequestTest) {
    success = true;
    std::string s = "GET / HTTP/1.1\r\n\r\n";
    Request *req = new Request(s);
    Response *resp = new Response();
    EchoHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_TRUE(success);
}

TEST_F(EchoHandlerTest, HTTP1Test) {
    success = true;
    std::string s = "GET / HTTP/1.0\r\n\r\n";
    Request *req = new Request(s);
    Response *resp = new Response();
    EchoHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(EchoHandlerTest, PostTest) {
    success = true;
    std::string s = "POST / HTTP/1.1\r\n\r\n";
    Request *req = new Request(s);
    Response *resp = new Response();
    EchoHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(EchoHandlerTest, NoNewLinesTest) {
    success = true;
    std::string s = "GET / HTTP/1.1";
    Request *req = new Request(s);
    Response *resp = new Response();
    EchoHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(EchoHandlerTest, BadSyntaxTest) {
    success = true;
    std::string s = "randomString";
    Request *req = new Request(s);
    Response *resp = new Response();
    EchoHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(EchoHandlerTest, EmptyTest) {
    success = true;
    std::string s = "";
    Request *req = new Request(s);
    Response *resp = new Response();
    EchoHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(EchoHandlerTest, JustNewLinesTest) {
    success = true;
    std::string s = "\r\n\r\n";
    Request *req = new Request(s);
    Response *resp = new Response();
    EchoHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

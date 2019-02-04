#include "gtest/gtest.h"
#include "request_handler.h"
#include "request.h"
#include "response.h"

class RequestHandlerTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() {}

    bool success;
};

TEST_F(RequestHandlerTest, CorrectRequestTest) {
    success = true;
    std::string s = "GET / HTTP/1.1\r\n\r\n";
    Request *req = new Request(s);
    Response *resp = new Response();
    RequestHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_TRUE(success);
}

TEST_F(RequestHandlerTest, HTTP1Test) {
    success = true;
    std::string s = "GET / HTTP/1.0\r\n\r\n";
    Request *req = new Request(s);
    Response *resp = new Response();
    RequestHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(RequestHandlerTest, PostTest) {
    success = true;
    std::string s = "POST / HTTP/1.1\r\n\r\n";
    Request *req = new Request(s);
    Response *resp = new Response();
    RequestHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(RequestHandlerTest, NoNewLinesTest) {
    success = true;
    std::string s = "GET / HTTP/1.1";
    Request *req = new Request(s);
    Response *resp = new Response();
    RequestHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(RequestHandlerTest, BadSyntaxTest) {
    success = true;
    std::string s = "randomString";
    Request *req = new Request(s);
    Response *resp = new Response();
    RequestHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(RequestHandlerTest, EmptyTest) {
    success = true;
    std::string s = "";
    Request *req = new Request(s);
    Response *resp = new Response();
    RequestHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

TEST_F(RequestHandlerTest, JustNewLinesTest) {
    success = true;
    std::string s = "\r\n\r\n";
    Request *req = new Request(s);
    Response *resp = new Response();
    RequestHandler handler(req, resp);
    success = handler.succeeded();
    EXPECT_FALSE(success);
}

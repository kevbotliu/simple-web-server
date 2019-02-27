#include "gtest/gtest.h"
#include "proxy_handler.h"
#include "request.h"
#include "reply.h"

class ProxyHandlerTest : public ::testing::Test 
{
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;
    bool success;

    bool setup_config() {
       bool parse_success = parser.Parse("../tests/proxy_config", &out_config);
       out_config.extract();
       return parse_success;
    }

    HandlerBlock get_handler() {
        for (auto handler : out_config.handler_blocks) {
            if (handler.name == "proxy") { return handler; }
        }
    }
};

TEST_F(ProxyHandlerTest, ValidResponseTest) {
    ASSERT_TRUE(setup_config());
    std::string s = "GET /proxy HTTP/1.1\r\n\r\n";
    const Request *req = new Request(s);
    std::string root_path = "proxy";
    success = ProxyHandler::create(out_config, root_path)
                ->HandleRequest(*req)->is_valid();
    EXPECT_TRUE(success);
}

TEST_F(ProxyHandlerTest, SuccessStatusCodeResponseTest) {
    ASSERT_TRUE(setup_config());
    std::string s = "GET /proxy HTTP/1.1\r\n\r\n";
    const Request *req = new Request(s);
    std::string root_path = "proxy";
    success = (ProxyHandler::create(out_config, root_path)
                ->HandleRequest(*req)->get_status_code() == 200);
    EXPECT_TRUE(success);
}

TEST_F(ProxyHandlerTest, HandlerPathTest) {
    ASSERT_TRUE(setup_config());
    HandlerBlock proxy_handler_block = get_handler();
    EXPECT_EQ("/ucla", proxy_handler_block.path);
}

TEST_F(ProxyHandlerTest, HandlerRemoteURLTest) {
    ASSERT_TRUE(setup_config());
    HandlerBlock proxy_handler_block = get_handler();
    EXPECT_EQ("www.ucla.edu", proxy_handler_block.remote_url);
}

TEST_F(ProxyHandlerTest, HandlerRemotePortTest) {
    ASSERT_TRUE(setup_config());
    HandlerBlock proxy_handler_block = get_handler();
    EXPECT_EQ("80", proxy_handler_block.remote_port);
}


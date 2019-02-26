#include "gtest/gtest.h"
#include "proxy_handler.h"
#include "request.h"
#include "reply.h"

//TODO: Proxy Tests
class ProxyHandlerTest : public ::testing::Test 
{
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;
    /*
    NginxConfig feedConfig(std::string filename) {
        parser.Parse(filename, &out_config);
        return out_config;
    }
    */
    bool setup_config() {
       // TODO: too long of string?
       bool yuck = parser.Parse("../tests/proxy_config", &out_config);
       std::cout << out_config.ToString() << std::endl;
       std::cout << "Statements: " << out_config.statements_.size() << std::endl;
       std::cout << "Statements: " << out_config.statements_.size() << std::endl;
       std::cout << "Size of handler blocks BEFORE extraction: " << out_config.handler_blocks.size() << std::endl;
       out_config.extract();
       std::cout << "Size of handler blocks AFTER extraction: " << out_config.handler_blocks.size() << std::endl;

       for (auto handler : out_config.handler_blocks) {
           std::cout << "HELLO" << std::endl;
           std::cout << handler.name << std::endl;
       }
       std::cout << "GOODBYE" << std::endl;
       return yuck;
    }

    bool success;
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

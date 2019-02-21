#include "gtest/gtest.h"
#include "dispatcher.h"

class DispatcherTest : public ::testing::Test {
  protected:
    NginxConfigParser parser;
    NginxConfig config;

    void SetUp() {}
    void TearDown() {}

    bool parse_from_string(const std::string config_string) {
      std::stringstream config_stream(config_string);
      return parser.Parse(&config_stream, &config);
    }
};



TEST_F(DispatcherTest, Create) {
  std::string normal_config = "listen 80; root /usr/src/projects/; handler static {location /static; root static_src;} handler echo {location /echo;}";
  ASSERT_TRUE(parse_from_string(normal_config));

  Dispatcher* dispatcher = Dispatcher::create(config);
  EXPECT_TRUE(dispatcher->handler_blocks.size() == 2);
  delete dispatcher;
}


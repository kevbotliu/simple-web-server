#include "gtest/gtest.h"
#include "config_parser.h"

class NginxConfigParserTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() {}

    NginxConfigParser parser;
    NginxConfig out_config;
    bool success;
};

TEST_F(NginxConfigParserTest, SimpleConfig) {
  success = parser.Parse("../tests/example_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NoConfig) {
  success = parser.Parse("../tests/no_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, EmptyConfig) {
  /* Reference https://piazza.com/class/jqn4lzgnhy0471?cid=64
     Whether empty config or empty block is valid or invalid is somewhat ambiguous.
     From Piazza post, these cases are open to our own interpretation.
  */
  success = parser.Parse("../tests/empty_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MissingOpeningBracket) {
  success = parser.Parse("../tests/missing_open_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MissingClosingBracket) {
  success = parser.Parse("../tests/missing_close_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MultipleBlocks) {
  success = parser.Parse("../tests/multiple_blocks_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, WithComment) {
  success = parser.Parse("../tests/includes_comment_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, HttpConfig) {
  success = parser.Parse("../tests/http_config", &out_config);
  EXPECT_TRUE(success);
}

// TEST_F(NginxConfigParserTest, NoPortConfig) {
//   success = parser.Parse("../tests/no_port_config", &out_config);
//   EXPECT_FALSE(success);
// }
#include "gtest/gtest.h"
#include "reply.h"

class ReplyTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() {}
};

TEST_F(ReplyTest, ValidWithDefaultArgs) {
  ReplyArgs args;

  Reply rep = Reply(args);
  EXPECT_TRUE(rep.is_valid());
}

TEST_F(ReplyTest, InvalidWithInvalidStatus) {
  ReplyArgs args;
  args.status_code = 900;

  Reply rep = Reply(args);
  ASSERT_TRUE(rep.to_string() == "");
  EXPECT_FALSE(rep.is_valid());
}

TEST_F(ReplyTest, ValidWithProperHeaders) {
  std::pair <std::string, std::string> ab ("A", "B");
  std::pair <std::string, std::string> cd ("C", "D");
  std::vector <std::pair<std::string, std::string>> h {ab, cd};

  ReplyArgs args;
  args.headers = h;

  Reply rep = Reply(args);
  std::string expected = "HTTP/1.1 200 OK\r\nA: B\r\nC: D\r\n\r\n";
  EXPECT_TRUE(rep.is_valid() && rep.to_string() == expected);
}

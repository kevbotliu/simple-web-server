#include "gtest/gtest.h"
#include "response.h"

class ResponseTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() {}

    bool success;
};

TEST_F(ResponseTest, invalidStatusTest) {

  Response *k = new Response();
  success = k->set_status(-1);

  EXPECT_FALSE(success);

}

TEST_F(ResponseTest, invalidateStringTest) {
  Response *k = new Response();
  k->invalidate();

  std::string str = k->Response::to_string();
  EXPECT_TRUE(str.compare("") == 0);
}

TEST_F(ResponseTest, setHeaderTest) {

  Response *k = new Response();
  k->set_version("version");
  k->set_status(200);
  k->set_body("body");

  std::pair <std::string, std::string> ab ("A", "B");
  std::pair <std::string, std::string> cd ("C", "D");
  std::vector <std::pair<std::string, std::string>> h {ab, cd};
  k->set_headers(h);

  std::string target = "version 200 OK\r\nA: B\r\nC: D\r\n\r\nbody";

  bool stringCheck = target.compare(k->Response::to_string()) == 0;
  bool sizeCheck = 34 == k->size();


  EXPECT_TRUE(sizeCheck && stringCheck);

}

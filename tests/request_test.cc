#include "gtest/gtest.h"
#include "request.h"

class RequestTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() {}
};



TEST_F(RequestTest, HeaderTest) {

  const std::string request = "GET / HTTP/1.1\r\nHeader: Example\r\n\r\n";
  Request k = Request(request);
  EXPECT_TRUE(k.is_valid());

}


TEST_F(RequestTest, pathTest) {
  const std::string request = "GET /HTTP /1.1\r\n\r\n";
  Request k = Request(request);  
  std::string path = k.get_path();
  EXPECT_TRUE(path.compare("/HTTP") == 0);
  
}

TEST_F(RequestTest, getHeaderTest) {

  const std::string request = "GET / HTTP/1.1\r\nHeader: Example\r\n\r\n";
  Request k = Request(request);
  std::vector<std::pair<std::string, std::string>> headers = k.get_headers();
  std::pair<std::string, std::string> theHeaderWeMade = headers.at(0);
  std::string headerName = theHeaderWeMade.first;
  std::string headerValue = theHeaderWeMade.second;

  EXPECT_TRUE(headerName.compare("Header") == 0 && headerValue.compare("Example") == 0);

}


#include "gtest/gtest.h"
#include "session_test.h"

class SessionTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() {}
    session_test s;
    bool success;
};

TEST_F(SessionTest, HandleReadTest){
    success = true;
    const std::string request = "GET / HTTP/1.1";
    std::string response = s.handle_read(request, 14);
    if (response != "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET / HTTP/1.1"){
        success = false;
    }
    EXPECT_TRUE(success);
}

TEST_F(SessionTest, NotEnoughByteToRead){
    success = true;
    const std::string request = "GET / HTTP/1.1";
    std::string response = s.handle_read(request, 1);
    if (response != ""){
        success = false;
    }
    EXPECT_TRUE(success);
}

TEST_F(SessionTest, HandleReadTest_2){
    success = true;
    const std::string request = "GET bogus";
    std::string response = s.handle_read(request, 9);
    if (response != ""){
        success = false;
    }
    EXPECT_TRUE(success);
}
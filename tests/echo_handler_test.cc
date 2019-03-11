// #include "gtest/gtest.h"
// #include "echo_handler.h"
// #include "request.h"
// #include "reply.h"

// class EchoHandlerTest : public ::testing::Test {
//   protected:
//     void SetUp() override {}
//     void TearDown() {}

//     bool success;
// };

// TEST_F(EchoHandlerTest, ValidResponseTest) {
//     success = true;
//     std::string s = "GET /echo HTTP/1.1\r\n\r\n";
//     const Request *req = new Request(s);
//     NginxConfig config;
//     std::string root_path = "echo";
//     success = EchoHandler::create(config, root_path)->HandleRequest(*req)->is_valid();
//     EXPECT_TRUE(success);
// }

// TEST_F(EchoHandlerTest, StatusCodeResponseTest) {
//     success = true;
//     std::string s = "GET /echo HTTP/1.1\r\n\r\n";
//     const Request *req = new Request(s);
//     NginxConfig config;
//     std::string root_path = "echo";
//     success = (EchoHandler::create(config, root_path)->HandleRequest(*req)->get_status_code()==200);
//     EXPECT_TRUE(success);
// }

// TEST_F(EchoHandlerTest, CorrectResponseTest) {
//     success = true;
//     std::string s = "GET /echo HTTP/1.1\r\n\r\n";
//     const Request *req = new Request(s);
//     NginxConfig config;
//     std::string root_path = "echo";
//     success = (EchoHandler::create(config, root_path)->HandleRequest(*req)->get_body().substr(0, 18)=="GET /echo HTTP/1.1");
//     EXPECT_TRUE(success);
// }
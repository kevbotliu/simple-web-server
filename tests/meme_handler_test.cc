// #include "gtest/gtest.h"
// #include "meme_handler.h"
// #include "request.h"
// #include "reply.h"

// class MemeHandlerTest : public ::testing::Test 
// {
//   protected:
//     NginxConfigParser parser;
//     NginxConfig config;
//     bool success;

//     bool setup_config() {
//        bool parse_success = parser.Parse("../server_config_mult", &config);
//        config.extractHandlerInfo();
//        return parse_success;
//     }
// };

// TEST_F(MemeHandlerTest, MemeListValidTest) {
//     ASSERT_TRUE(setup_config());
//     std::string s = "GET /meme/new HTTP/1.1\r\n\r\n";
//     const Request *req = new Request(s);
//     std::string root_path = "/";
//     success = (MemeHandler::create(config, root_path)
//                 ->HandleRequest(*req) != nullptr);
//     EXPECT_TRUE(success);
// }

// // TEST_F(MemeHandlerTest, MemeListStatusCodeTest) {
// //     ASSERT_TRUE(setup_config());
// //     std::string s = "GET /meme/list HTTP/1.1\r\n\r\n";
// //     const Request *req = new Request(s);
// //     std::string root_path = "/";
// //     success = (MemeHandler::create(config, root_path)
// //                 ->HandleRequest(*req)->get_status_code() == 200);
// //     EXPECT_TRUE(success);
// // }

// TEST_F(MemeHandlerTest, MemeNewValidTest) {
//     ASSERT_TRUE(setup_config());
//     std::string s = "GET /meme/new HTTP/1.1\r\n\r\n";
//     const Request *req = new Request(s);
//     std::string root_path = "/";
//     success = (MemeHandler::create(config, root_path)
//                 ->HandleRequest(*req) != nullptr);
//     EXPECT_TRUE(success);
// }

// TEST_F(MemeHandlerTest, MemeNewStatusCodeTest) {
//     ASSERT_TRUE(setup_config());
//     std::string s = "GET /meme/new HTTP/1.1\r\n\r\n";
//     const Request *req = new Request(s);
//     std::string root_path = "/";
//     success = (MemeHandler::create(config, root_path)
//                 ->HandleRequest(*req)->get_status_code() == 200);
//     EXPECT_TRUE(success);
// }
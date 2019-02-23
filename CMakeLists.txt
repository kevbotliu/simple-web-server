cmake_minimum_required(VERSION 3.10.0)
project(bbk-simple-echo-server)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system log)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(include)

# Libraries
add_library(server_lib src/server.cc)
add_library(session_lib src/session.cc)
add_library(config_parser_lib src/config_parser.cc)
add_library(request_lib src/request.cc)
add_library(reply_lib src/reply.cc)
add_library(handler_factory_lib src/handler_factory.cc)
add_library(pathfinder_lib src/pathfinder.cc)

add_library(echo_handler_lib src/echo_handler.cc)
add_library(status_handler_lib src/status_handler.cc)
add_library(not_found_handler_lib src/not_found_handler.cc)
add_library(proxy_handler src/proxy_handler.cc)

add_library(static_handler_lib src/static_handler.cc)
target_link_libraries(static_handler_lib pathfinder_lib)

add_library(dispatcher_lib src/dispatcher.cc)
target_link_libraries(dispatcher_lib pathfinder_lib)

add_library(logger_lib src/logger.cc)
target_link_libraries(logger_lib Boost::log_setup Boost::log)

add_library(shared_lib src/shared.cc)

# Link libraries
add_executable(main src/main.cc)
target_link_libraries(
	main
	server_lib
	session_lib
	request_lib
	reply_lib
	handler_factory_lib
	dispatcher_lib
	pathfinder_lib
	echo_handler_lib
	static_handler_lib
	status_handler_lib
	not_found_handler_lib
	proxy_handler
	config_parser_lib
	Boost::system
	logger_lib
	shared_lib
)
add_executable(config_parser src/config_parser_main.cc)
target_link_libraries(config_parser config_parser_lib)

# TODO(!): Update test executable name, srcs, and deps
add_executable(config_parser_test tests/config_parser_test.cc)
target_link_libraries(config_parser_test config_parser_lib gtest_main)

add_executable(request_test tests/request_test.cc)
target_link_libraries(request_test request_lib gtest_main Boost::system)

add_executable(reply_test tests/reply_test.cc)
target_link_libraries(reply_test reply_lib gtest_main Boost::system)

# add_executable(dispatcher_test tests/dispatcher_test.cc)
# target_link_libraries(dispatcher_test dispatcher_lib gtest_main Boost::system)

add_executable(echo_handler_test tests/echo_handler_test.cc)
target_link_libraries(echo_handler_test request_lib reply_lib echo_handler_lib gtest_main Boost::system)

add_executable(static_handler_test tests/static_handler_test.cc)
target_link_libraries(static_handler_test request_lib reply_lib static_handler_lib gtest_main Boost::system)

# TODO(!): add tests for proxy_handler

# add_test(integration_test, ../tests/IntegrationTest [WORKING_DIRECTORY ../tests])

# TODO(!): Update with test binary
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(reply_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# gtest_discover_tests(dispatcher_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(echo_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(static_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)


# TODO(!): Add library under TARGETS and TESTS and lib_test to generate_coverage_report
include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(
	TARGETS
		main
		server_lib
		session_lib
		config_parser_lib
		request_lib
		reply_lib
		handler_factory_lib
		dispatcher_lib
		pathfinder_lib
		echo_handler_lib
		static_handler_lib
		status_handler_lib
		shared_lib
		not_found_handler_lib
	TESTS
		config_parser_test
		request_test
		reply_test
		static_handler_test
		# dispatcher_test
		echo_handler_test
)

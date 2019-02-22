## Simple Server
By team google-plus-plus

# Source Code Layout
The source code is located in the /src directory, and the header files are located in the /include directory. In general, a C++ file located in /src will have the same filename, save for the extension, for its corresponding header file in /include. Exceptions are code that creates executables (config_parser_main.cc, main.cc) and templates for the request handlers and session tests (request_handler.h, session_test.h).

# Building
Create a build directory in the root of the source code to make things much easier. To build the code, run “cmake ..” then “make” inside the build directory.
Note that Boost::regex is required for successful compilation. Make sure that libboost-log-dev and libboost-regex-dev are already installed.

# Running
The executable is located in the /bin directory of the build directory, named main. It _must_ be run from the build directory or else handling will not work. The argument for this executable is a nginx-style configuration file, for example server_config_mult located in the root directory (or .. from the build directory).

Assuming that the program has been built with aforementioned instructions, the program can thus be run like this:

./bin/main ../server_config_mult

After that, open a web browser and go to:

{IP ADDRESS}:{PORT NUMBER}

The port number is defined in the configuration file, so for our example it should be localhost:8080. This will bring up an echo.

To access static files, go to:

{IP ADDRESS}:{PORT NUMBER}/{STATIC FOLDER}/{STATIC FILE}

For instance, to access blah.txt in localhost, we go to:

localhost:8080/static/blah.txt

# Testing
Run “make test” inside the build directory. The output is a series of test results, and a summary of the tests at the end.

# Adding request handlers
Create a new source file in the /src directory, and its corresponding header file in the /include directory. Make sure to include request_handler.h, and create a class that inherits from the RequestHandler class.

The header should contain two public functions: create and HandleRequest. Create simply should return a new handler of its own, and HandleRequest should define how the handler handles the request.

As usual, the constructor function should also be defined in both header and source file.

After finishing creating the request handler, you must modify CMakeLists.txt in the project root directory. First, you should add the code. For example:

add_library(sample_handler_lib src/sample_handler.cc)

Then, target other libraries to it if necessary:

target_link_libraries(sample_handler_lib another_lib)

Finally, if testing, add the name of the library (i.e. sample_handler_lib in our example) under TARGETS in generate_coverage_report at the end of the file.

Lastly, add the option for this handler in src/handler_factory.cc. It should imitate this structure:

if (name == “sample”) return std::unique_ptr<RequestHandler>(SampleHandler::create (config, root_path));

# Overview of our handler design

All handler objects inherit from a pure abstract class “RequestHandler”, defined in /$PROJECT/include/request_handler.h

An RequestHandler object has 2 protected members:
-- NginxConfig config: stores the Nginx configuration parsed from the customized configuration file.
-- string root_path_: stores the root path of the request, also specified in the customized configuration file.

An RequestHandler object also has a virtual member function, which is implemented by each descendent class explicitly:
-- virtual std::unique_ptr<Reply> HandleRequest(const Request& request){}
HandleRequest takes in the client request as a parameter, and returns a std::unique_ptr<Reply> type. Reply object is a self-implemented class defined in /$PROJECT/include/reply.h. HandleRequest should process the request and compose the response corresponding to the objectives of each specific descendent handler.

# An Example

Let’s look at a simple example -- EchoHandler. An EchoHandler inherits from the RequestHandler. Its objective is to echo the client request in the response body.

In addition to RequestHandler’s members, EchoHandler has another member function, create(). This function simply initiate an EchoHandler instance by calling a private constructor. 

EchoHandler also implements member function “HandleRequest”, so that when composing a response, it prints the request body in the response.

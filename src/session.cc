#include "session.h"
#include "shared.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>

session::session(boost::asio::io_service& io_service, Dispatcher* dispatcher)
    : socket_(io_service),
      dispatcher_(dispatcher),
      log()
  {
  }

tcp::socket& session::socket()
  {
    return socket_;
  }

void session::start()
  {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        boost::bind(&session::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

void session::handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    { 
      std::string s = data_;
      std::string request_str = s.substr(0, bytes_transferred);

      Request req = Request(request_str);
      std::unique_ptr<RequestHandler> handler = dispatcher_->dispatch(req);

      std::unique_ptr<Reply> rep;
      rep = handler->HandleRequest(req);

      std::string reply_str = "";  
      if (!rep) std::cout << "Failed creating reply object\n";
      else reply_str = rep->to_string();

      RequestHistory::history.push_back(std::make_pair(req.get_path(), rep->get_status_code()));

      boost::asio::async_write(socket_,
          boost::asio::buffer(reply_str, reply_str.length()),
          boost::bind(&session::handle_write, this,
            boost::asio::placeholders::error));
    }
    else delete this;
  }

void session::handle_write(const boost::system::error_code& error)
  {
    delete this;
    // if (!error)
    // {
    //   socket_.async_read_some(boost::asio::buffer(data_, max_length),
    //       boost::bind(&session::handle_read, this,
    //         boost::asio::placeholders::error,
    //         boost::asio::placeholders::bytes_transferred));
    // }
    // else
    // {
    //   delete this;
    // }
  }


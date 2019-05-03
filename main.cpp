#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <windows.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "ActionRegistry.h"
#include "BarkAction.h"

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = net::ip::tcp;

void PrintDNSEntry(const boost::asio::ip::basic_resolver_entry<class boost::asio::ip::tcp>& entry) {
  std::cout << entry.endpoint() << std::endl;
}

void DNSLookup(const std::string &hostname) {
  boost::asio::io_context ioc;
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::asio::ip::tcp::resolver::query query(hostname, "80");

  try {
    const auto entries = resolver.resolve(query);
    std::for_each(
      entries.cbegin(),
      entries.cend(),
      PrintDNSEntry
      // [](const auto &x) {
      //   std::cout << typeid(x).name() << std::endl;
      // }
    );
  } catch(const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}

void ConnectToRestService() {
  const std::string host("todos-yyz-service.herokuapp.com");
  boost::asio::io_context ioc;
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::asio::ip::tcp::resolver::query query(host, "80");

  const auto address = resolver.resolve(query);

  std::cout << "Connecting to " << address.begin()->endpoint() << "..." << std::endl;

  beast::tcp_stream stream(ioc);

  const auto endpoint = address.cbegin()->endpoint();

  try {
    stream.connect(address);
    std::cout << "Connected to " << endpoint << "!" << std::endl;
  } catch(const std::exception &e) {
    std::cout
      << "Could not connect to "
      << endpoint
      << ": "
      << e.what();
    return;
  }

  http::request<http::string_body> req(http::verb::get, "/lists", 11);

  req.set(http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  // Send the HTTP 1.1 GET request to the remote host
  try {
    http::write(stream, req);
  } catch(const std::exception &e) {
    std::cout
      << "Could not send HTTP Request: "
      << e.what()
      << std::endl;
    return;
  }

  std::cout << "Sent HTTP Request!" << std::endl;

  // This buffer is used for reading and must be persisted
  beast::flat_buffer buffer;

  // Declare a container to hold the response
  http::response<http::dynamic_body> res;

  // Receive the HTTP response
  http::read(stream, buffer, res);

  // Write the message to standard out
  std::cout
    << std::endl
    << "HTTP Response:"
    << std::endl
    << "----------------------------------------------------------"
    << std::endl
    << res
    << std::endl
    << "----------------------------------------------------------"
    << std::endl;

  // Gracefully close the socket
  beast::error_code ec;
  stream.socket().shutdown(tcp::socket::shutdown_both, ec);

  if(ec && ec != beast::errc::not_connected)
      throw beast::system_error{ec};
}

void Actions()
{
  ActionRegistry actions;
  BarkAction barkAction;

  actions.Register("bark", &barkAction);
  actions.Execute("bark");
}

void DetermineMeaningOfLifeTheUniverseAndEverything(boost::function<void(int)> callback)
{
  /*
   * Long running operation aka Deep Thought takes 7.5 million years to compute
   * the answer to the meaning of life, the universe and everything...
   */
  std::cout << "Computing the answer to the meaning of life, the universe and everything..." << std::endl;
  callback(42);
}

void OnAnswerComputed(const int answer)
{
  std::cout << "Survey says: " << answer << std::endl;
}

void HitchhikersGuideToTheGalaxy()
{
  DetermineMeaningOfLifeTheUniverseAndEverything(boost::bind(&OnAnswerComputed, _1));
}

int main()
{
  HitchhikersGuideToTheGalaxy();
  //Actions();
  //DNSLookup("www.google.com");
  //ConnectToRestService();
}

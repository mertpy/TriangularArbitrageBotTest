#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <mutex>
#include "json/nlohmann/json.hpp"
#include <string>
#include <cmath>
#include <curl/curl.h>
#include <thread>
#include <fstream>
#include <chrono>
#pragma

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
namespace http = boost::beast::http;
using tcp = net::ip::tcp;
using json = nlohmann::json;
using namespace std;



mutex ws_mutex;//for WebSocket
unique_ptr<websocket::stream<beast::ssl_stream<beast::tcp_stream>>> ws_ptr;
bool connected = false;
net::io_context ioc;
ssl::context ctx(ssl::context::tlsv12_client); 

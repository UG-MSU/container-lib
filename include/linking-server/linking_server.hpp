#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = boost::filesystem;
using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

class client {
private:
  io_service service;
  ip::tcp::endpoint ep;
  static size_t read_complete(char *buff, const error_code &err, size_t bytes);
  static std::string copy(std::string path2file);
  void sync_echo(std::string msg);
  std::string vec2str(std::vector<std::string> vec);
  std::string read_from_buff(ip::tcp::socket & sock);
  void send_file(ip::tcp::socket & sock);
  std::string file2string(std::string path2file);

public:
  client(int port = 8001, std::string local_add = "127.0.0.1");
  void start(std::vector<std::string> args, std::string path2file, std::string path2input);
};

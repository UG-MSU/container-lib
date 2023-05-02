/*

СИНХРОННЫЙ СЕРВЕР, КОТОРЫЙ КОНТРОЛИРУЕТ ЗАПУСК ПРОГРАММЫ ВНУРИ DOCKER КОНТЕЙНЕРА

*/

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

class server {
private:
  io_service service;
  ip::tcp::endpoint ep;
  static size_t read_complete(char *buff, const error_code &err, size_t bytes);
  std::string start_execution(std::string command);
  static std::string file2string(std::string path2file);
  void string2file(std::string output); 
  std::string read_from_buff(ip::tcp::socket & sock);
  std::string read_file_bytes(std::string name);

public:
  server(int port, std::string local_add = "127.0.0.1");
  void handle_connections();
};


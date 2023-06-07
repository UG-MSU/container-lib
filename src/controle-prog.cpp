#include "container-lib/container-lib.hpp"
server::server(int port, std::string local_add) {
  ip::tcp::endpoint ep(ip::address::from_string(local_add), port);
  this->ep = ep;
}

size_t server::read_complete(char *buff, const error_code &err, size_t bytes) {
  if (err)
    return 0;
  bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
  return found ? 0 : 1;
}

std::string server::start_execution(std::string command) {
  command += " < input.txt > log.txt\n";
  std::cerr << command;
  std::system(command.c_str());
  std::string output = file2string("log.txt");
  return output;
}

void server::handle_connections() {
  ip::tcp::acceptor acceptor(service, ep);
  while (true) {
    ip::tcp::socket sock(service);
    acceptor.accept(sock);
    std::string command = read_from_buff(sock);
    command.pop_back();
    std::string output = start_execution(command);
    sock.write_some(buffer(output));
    sock.close();
  }
}

std::string server::file2string(std::string path2file) {
  std::ifstream file(path2file);
  std::string tmp;
  std::stringstream output;

  if (file.is_open()) {
    while (getline(file, tmp)) {
      output << tmp << "\n";
    }
    output << std::endl;
    file.close();
  }
  return output.str();
}

void server::string2file(std::string output) {
  std::ofstream out("input.txt");
	out << output;
	out.close();
  return;
}

std::string server::read_from_buff(ip::tcp::socket & sock) {
  int bytes;
  char buff[65536];
  bytes = read(sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));
  std::string output(buff, bytes);
  return output;
}

std::string server::read_file_bytes(std::string name) {
  std::ifstream fl(name);
  fl.seekg(0, std::ios::end);
  size_t len = fl.tellg();
  char * file_bytes = new char[len];
  fl.seekg(0, std::ios::beg);
  fl.read(file_bytes, len);
  fl.close();
  return std::string(file_bytes);
}

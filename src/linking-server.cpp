#include "container-lib/container-lib.hpp"

void ContainerLib::client::start(std::vector<std::string> args, std::string path2file, std::string path2input) {
  std::string directory_dist_file = copy(path2file);
  std::string directory_dist_input = copy(path2input);
  std::string str_args = vec2str(args);
  std::string ans = str_args + directory_dist_file;
  sync_echo(ans);
}

size_t ContainerLib::client::read_complete(char *buf, const error_code &err, size_t bytes) {
  if (err)
    return 0;
  bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
  return found ? 0 : 1;
}

ContainerLib::client::client(int port, std::string local_add) {
  ip::tcp::endpoint ep(ip::address::from_string(local_add), port);
  this->ep = ep;
}

void ContainerLib::client::sync_echo(std::string msg) {
  ip::tcp::socket sock(service);
  sock.connect(ep);
  msg.push_back('\n');
  sock.write_some(buffer(msg));
  char buf[1024];
  int bytes = read(sock, buffer(buf), boost::bind(read_complete, buf, _1, _2));
  std::string output(buf, bytes - 1); 
  std::cout << output << std::endl;
  sock.close();
}

std::string ContainerLib::client::copy(std::string path2file) {
  fs::path file_source = path2file;
  if (!fs::is_regular_file(file_source)) {
    std::cout << "Such file is not exist" << std::endl;
    return "";
  }

  fs::path directory_dest = fs::current_path();
  directory_dest /= "exec_dir";

  if (!fs::is_directory(directory_dest)) {
    fs::create_directory(directory_dest);
  }

  directory_dest /= file_source.filename();

  try {
    fs::copy_file(file_source, directory_dest,
                  fs::copy_option::overwrite_if_exists);
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return directory_dest.c_str();
}

std::string ContainerLib::client::vec2str(std::vector<std::string> vec) {
  std::stringstream output;
  for (const auto &x : vec) {
    output << x << " ";
  }
  return output.str();
}

std::string ContainerLib::client::read_from_buff(ip::tcp::socket & sock) {
  int bytes;
	char buff[65536];
	bytes = read(sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));
	std::string output(buff, bytes);
	return output;
}

std::string ContainerLib::client::file2string(std::string path2file) {
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

#include "docker-lib/docker-lib.hpp"

void DockerLib::container::init_container() {
  Docker client = Docker();
  /*JSON_DOCUMENT doc = client.list_containers(true); 
  std::cout << jsonToString(doc) << std::endl;*/
  std::cout << "HELLO, I`M DOCKER!" << std::endl;
}

void DockerLib::container::delete_container() {}

void DockerLib::container::start(std::string path_to_program, std::string args) {}

std::string DockerLib::container::get_output() {return "";}
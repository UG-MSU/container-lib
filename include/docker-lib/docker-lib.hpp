#include "docker-cpp/docker.h"
#include <iostream>

namespace DockerLib {

class Docker {
  private:
    void init_docker();
    void delete_docker();

  public:
    void start(std::string path_to_program, std::string args);
    std::string get_output();
};

} // namespace DockerLib

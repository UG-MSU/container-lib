#include "docker-cpp/docker.h"
#include <iostream>

namespace DockerLib {

class container {
  private:
    //void init_container();
    void delete_container();

  public:
    void init_container();
    void start(std::string path_to_program, std::string args);
    std::string get_output();
};

} // namespace DockerLib

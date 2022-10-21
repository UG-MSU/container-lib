#include <iostream>

namespace ContainerLib {
  using time_t = size_t;

  pid_t main_proc, slave_proc;

  struct launch_options {
    time_t time;
    size_t forks_amount;
    size_t forks_threshold;
  };

  pid_t create_processes();
  bool flow_conrol();
  void start(std::string path_to_binary, launch_options);
}

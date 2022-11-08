#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace ContainerLib {
  using time_t = size_t;

  pid_t main_proc, slave_proc;

  struct launch_options {
    time_t time;
    size_t forks_amount;
    size_t forks_threshold;
  } launch_options;

  void create_processes();
  bool sync();
  void start(std::string path_to_binary);
}

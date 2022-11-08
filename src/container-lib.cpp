#include "../include/container-lib/container-lib.hpp"

void ContainerLib::create_processes() {
  main_proc = frok();
  if (get_pid() != main_proc)
    slave_proc = fork();
  return;
}

bool ContainerLib::sync() {
  if (getpid() != main_proc && getpid() != slave_proc) {
    wait(nullptr);
    wait(nullptr);
  }
  return 1; // the stub, have to change
}

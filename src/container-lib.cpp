#include "../include/container-lib/container-lib.hpp"

pid_t ContainerLib::create_processes() {
  main_proc = getpid();
  slave_proc = fork();
  return slave_proc;
}

bool ContainerLib::sync() {
  if (getpid() != main_proc && getpid() != slave_proc) {
    wait(nullptr);
    wait(nullptr);
  }
  return 1; // the stub, have to change
}

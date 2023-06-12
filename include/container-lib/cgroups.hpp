#ifndef CGROUPS_H
#define CGROUPS_H
#include "container-lib/exception.hpp"
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define FILE_EXISTS(file) (access((file).c_str(), F_OK) == 0)
#define SAFE(func, call)                                                       \
    if ((call) < 0) {                                                          \
        throw ContainerLib::ContainerException(std::string(func) + ": " +      \
                                               std::string(strerror(errno)));  \
    }
#define intstr(s) std::to_string(s)
namespace ContainerLib {
class Cgroup {
  private:
    void echo_to_file(std::string path, std::string text);
    int cgroup_verison();
    const int64_t CGROUPV2_MAGIC = 1667723888;
    const int64_t CGROUPV1_MAGIC = 2613483;
    const std::string CGROUP_PATH = "/sys/fs/cgroup";
    const std::string MAIN_CGROUP_PATH = "/sys/fs/cgroup/yats";

  public:
    std::string CGROUP_ID;
    ~Cgroup() { deinit(); }
    void init(uint64_t MEM_SIZE, double TOTAL_CPU_PERCENTAGE,
              std::string _CGROUP_ID, int CPU);
    void add_process(pid_t pid);
    void deinit();
};
} // namespace ContainerLib
#endif

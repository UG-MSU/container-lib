#ifndef CGROUPS_H
#define CGROUPS_H
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
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

void echo_to_file(std::string path, std::string text);
int cgroup_verison(const char CGROUP_PATH[50]);
void init_cgroup(uint64_t MEM_SIZE, double TOTAL_CPU_PERCENTAGE,
                 const char CGROUP_ID[20], int CPU);
void add_to_cgroup(pid_t pid, const char CGROUP_ID[20]);
void deinit_cgroup(const char CGROUP_ID[20]);
#define FILE_EXISTS(file) access(file, F_OK) == 0
#define SAFE(func, call)                                                       \
    if ((call) < 0) {                                                          \
        perror(func);                                                          \
        exit(1);                                                               \
    }
#define SAFE_FS(fstream, file)                                                 \
    fstream.open(file);                                                        \
    if (!fstream) {                         \
        std::cerr << "if triggered";                                            \                                                      
        exit(1);                                                               \
    }                   

const int64_t CGROUPV2_MAGIC = 1667723888;
const int64_t CGROUPV1_MAGIC = 2613483;
const char CGROUP_PATH[20] = "/sys/fs/cgroup";
const char MAIN_CGROUP_PATH[20] = "/sys/fs/cgroup/yats";

#endif

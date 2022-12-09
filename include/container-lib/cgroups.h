#ifndef CGROUPS_H
#define CGROUPS_H
#include <errno.h>
#include <fcntl.h>
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

#define SAFE(func, call)                                                       \
    if ((call) < 0) {                                                          \
        perror(func);                                                          \
        exit(1);                                                               \
    }

const int64_t CGROUPV2_MAGIC = 1667723888;
const int64_t CGROUPV1_MAGIC = 2613483;
const char CGROUP_PATH[20] = "/sys/fs/cgroup";
const char MAIN_CGROUP_PATH[20] = "/sys/fs/cgroup/yats";

void echo_to_file(const char *path, const char *text, int len);
int cgroup_verison(const char CGROUP_PATH[50]);
void init_cgroup(long long MEM_SIZE, float TOTAL_CPU_PERCENTAGE,
                 char CGROUP_ID[20], int CPU);
void add_to_cgroup(pid_t pid, char CGROUP_ID[20]);
void deinit_cgroup(char CGROUP_ID[20]);
#endif CGROUPS_H
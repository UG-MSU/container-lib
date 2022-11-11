#include<sys/types.h>
#include<sys/wait.h>
#include <sys/mount.h>
#include <errno.h>
#include<stdio.h>
#include <stdlib.h>
#include <sstream>
#include<unistd.h>
void init_cgroup(int _MEM_SIZE, float _TOTAL_CPU_PERCENTAGE, char CGROUP_ID[20]) {
    std::stringstream ss;
    ss << "../utils/init_cgroup.sh " << _MEM_SIZE << " " << _TOTAL_CPU_PERCENTAGE << " " << CGROUP_ID;
        system(ss.str().c_str());
    }
void add_to_cgroup(pid_t pid, char CGROUP_ID[20]) {
    std::stringstream ss;
    ss << "../utils/add_to_cgroup.sh " << pid << " " << CGROUP_ID;
        system(ss.str().c_str());
}
void deinit_cgroup(char CGROUP_ID[20]) {
    std::stringstream ss;
    ss << "../utils/deinit_cgroup.sh " << CGROUP_ID;
        system(ss.str().c_str());
}


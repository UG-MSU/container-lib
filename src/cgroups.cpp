#include "container-lib/cgroups.hpp"


void echo_to_file(std::string path, std::string text) {
    int fd;
    int len = text.size();
    SAFE(("file open error "+path).c_str(), fd = open(path.c_str(), O_WRONLY));
    if (len != write(fd, text.c_str(), len)) {
        perror("write error");
    }
    SAFE("file close error", close(fd));

}

int cgroup_verison() {
    struct statfs _buf;
    SAFE("statfs error", statfs(CGROUP_PATH.c_str(), &_buf));
    int64_t fs_type = _buf.f_type;
    if (fs_type == CGROUPV2_MAGIC) // cgroupv2 fs magic number
        return 2;
    if (fs_type == CGROUPV1_MAGIC) // cgroupv1 fs magic number
        return 1;
    else
        mount("cgroup2", CGROUP_PATH.c_str(), "cgroup2", 0, NULL);
    return 2; // not a cgroup fs
}
void init_cgroup(uint64_t MEM_SIZE, double TOTAL_CPU_PERCENTAGE,
                 std::string CGROUP_ID, int CPU) {
    int _cversion = cgroup_verison();
    std::string cgroup = MAIN_CGROUP_PATH+"/"+CGROUP_ID;
    switch (_cversion) {
    case 2: { // cgroup v2
        if (!FILE_EXISTS(MAIN_CGROUP_PATH))
            SAFE("mkdir err:", mkdir(MAIN_CGROUP_PATH.c_str(), 0777));
        if (!FILE_EXISTS(MAIN_CGROUP_PATH+"/"+CGROUP_ID)) {
            SAFE("mkdir err:", mkdir((MAIN_CGROUP_PATH+"/"+CGROUP_ID).c_str(), 0777));
        } else {
            SAFE("rmdir err:", rmdir((MAIN_CGROUP_PATH+"/"+CGROUP_ID).c_str()));
            SAFE("mkdir err:", mkdir((MAIN_CGROUP_PATH+"/"+CGROUP_ID).c_str(), 0777));
        }
        echo_to_file(MAIN_CGROUP_PATH+"/cgroup.subtree_control", "+cpu +memory +cpuset"); // enable controllers
        echo_to_file(CGROUP_PATH+"/cgroup.subtree_control", "+cpu +cpuset");
        echo_to_file(cgroup+"/cpuset.cpus", intstr(CPU)); // set random cpu core
        echo_to_file(cgroup+"/cpu.max", intstr((uint64_t)(1000000 * TOTAL_CPU_PERCENTAGE)) +" "+intstr(1000000)); // set max cpu time);
        echo_to_file(cgroup+"/memory.swap.max", "0"); //  disable memory swap
        echo_to_file(cgroup+"/memory.max", intstr(MEM_SIZE * 1024)); // set max memory
        break;
    }
    case 1: { // cgroup v1
        mkdir((CGROUP_PATH+"/cpuset/yats").c_str(), 0700);
        mkdir((CGROUP_PATH + "/cpuset/yats/" + CGROUP_ID).c_str(), 0700);
        echo_to_file(cgroup+"/cpuset.cpus", intstr(CPU)); // set random cpu core
        echo_to_file((cgroup+"/cpu.max").c_str(), intstr((uint64_t)(1000000 * TOTAL_CPU_PERCENTAGE)) +" "+intstr(1000000)); // set max cpu time
        mkdir((CGROUP_PATH+"/memory/yats").c_str(), 0700);
        mkdir((CGROUP_PATH+"/memory/yats/"+CGROUP_ID).c_str(), 0700);
        echo_to_file((cgroup+"/memory.swap.max").c_str(), "0"); //  disable memory swap
        echo_to_file((cgroup+"/memory.max").c_str(), intstr(MEM_SIZE * 1024)); // set max memory
        break;
    }
    case 0: // no cgroup mounted (error)
        printf("no cgroup mounted");
        exit(1);
    }
}
void add_to_cgroup(pid_t pid, std::string CGROUP_ID) {
    int _cversion = cgroup_verison();
    switch (_cversion) {
    case 2: {
        chmod((MAIN_CGROUP_PATH + "/" + CGROUP_ID + "/cgroup.procs").c_str(), 0777);
        echo_to_file(MAIN_CGROUP_PATH + "/" + CGROUP_ID + "/cgroup.procs", intstr(pid));
        break;
    }
    case 1: {
        echo_to_file(CGROUP_PATH + "/cpuset/yats/" + CGROUP_ID + "/tasks", intstr(pid));
        echo_to_file(CGROUP_PATH + "/cpu/yats/" + CGROUP_ID + "/tasks", intstr(pid));
        echo_to_file(CGROUP_PATH + "/memory/yats/" + CGROUP_ID + "/tasks", intstr(pid));
        break;
    }
    case 0: // no cgroup mounted (error)
        printf("no cgroup mounted");
        exit(1);
    }
}

void deinit_cgroup(std::string CGROUP_ID) {
    SAFE("deinit err", rmdir((MAIN_CGROUP_PATH + "/" + CGROUP_ID).c_str()));
}

#include "container-lib/cgroups.hpp"

void echo_to_file(const char *path, const char *text, int len) {
    int fd;
    SAFE("file open error", fd = open(path, O_WRONLY));
    if (len != write(fd, text, len)) {
        perror("write error");
    }
    SAFE("file close error", close(fd));
}

int cgroup_verison(const char CGROUP_PATH[50]) {
    struct statfs _buf;
    SAFE("statfs error", statfs(CGROUP_PATH, &_buf));
    int64_t fs_type = _buf.f_type;
    if (fs_type == CGROUPV2_MAGIC) // cgroupv2 fs magic number
        return 2;
    if (fs_type == CGROUPV1_MAGIC) // cgroupv1 fs magic number
        return 1;
    else
        mount("cgroup2", CGROUP_PATH, "cgroup2", 0, NULL);
    return 2; // not a cgroup fs
}

void init_cgroup(uint64_t MEM_SIZE, double TOTAL_CPU_PERCENTAGE,
                 const char CGROUP_ID[20], int CPU) {
    // const int cpu_n = get_nprocs();
    // const int rand_cpu = rand() % cpu_n;
    const int rand_cpu = CPU;
    int _cversion = cgroup_verison(CGROUP_PATH);
    char _cgroup[100];
    char __path[100];
    char _str_cpu[3];
    char _cpumax[30];
    char _memorymax[30];
    sprintf(_str_cpu, "%i", rand_cpu);
    sprintf(_cpumax, "%lli %i", (uint64_t)(1000000 * TOTAL_CPU_PERCENTAGE),
            1000000);
    sprintf(_memorymax, "%lli", MEM_SIZE * 1024);
    switch (_cversion) {
    case 2: { // cgroup v2
        sprintf(_cgroup, "%s/%s", MAIN_CGROUP_PATH, CGROUP_ID);
        mkdir(MAIN_CGROUP_PATH, 0777);
        SAFE("mkdir err:", mkdir(_cgroup, 0777));
        chmod(_cgroup, 0777);
        char _subtreepath[100];
        char __subtreepath[100];
        sprintf(_subtreepath, "%s/cgroup.subtree_control", MAIN_CGROUP_PATH);
        sprintf(__subtreepath, "%s/cgroup.subtree_control", CGROUP_PATH);
        echo_to_file(__subtreepath, "+cpu +cpuset", 12);
        echo_to_file(_subtreepath, "+cpu +memory +cpuset",
                     20); // enable controllers
        sprintf(__path, "%s/cpuset.cpus", _cgroup);
        echo_to_file(__path, _str_cpu, strlen(_str_cpu)); // set random cpu core
        sprintf(__path, "%s/cpu.max", _cgroup);
        echo_to_file(__path, _cpumax, strlen(_cpumax)); // set max cpu time
        sprintf(__path, "%s/memory.swap.max", _cgroup);
        echo_to_file(__path, "0", 1); //  disable memory swap
        sprintf(__path, "%s/memory.max", _cgroup);
        echo_to_file(__path, _memorymax, strlen(_memorymax)); // set max memory
        break;
    }
    case 1: { // cgroupv1
        sprintf(_cgroup, "%s/cpuset/yats", CGROUP_PATH);
        mkdir(_cgroup, 0700);
        sprintf(_cgroup, "%s/cpuset/yats/%s", CGROUP_PATH, CGROUP_ID);
        mkdir(_cgroup, 0700);
        sprintf(__path, "%s/cpuset.cpus", _cgroup);
        echo_to_file(__path, _str_cpu, strlen(_str_cpu)); // set random cpu core
        sprintf(_cgroup, "%s/cpus/yats", CGROUP_PATH);
        mkdir(_cgroup, 0700);
        sprintf(_cgroup, "%s/cpu/yats/%s", CGROUP_PATH, CGROUP_ID);
        mkdir(_cgroup, 0700);
        sprintf(__path, "%s/cpu.max", _cgroup);
        echo_to_file(__path, _cpumax, strlen(_cpumax)); // set max cpu time
        sprintf(_cgroup, "%s/memory/yats", CGROUP_PATH);
        mkdir(_cgroup, 0700);
        sprintf(_cgroup, "%s/memory/yats/%s", CGROUP_PATH, CGROUP_ID);
        mkdir(_cgroup, 0700);
        sprintf(__path, "%s/memory.swap.max", _cgroup);
        echo_to_file(__path, "0", 1); //  disable memory swap
        sprintf(__path, "%s/memory.max", _cgroup);
        echo_to_file(__path, _memorymax, strlen(_memorymax)); // set max memory
        break;
    }
    case 0: // no cgroup mounted (error)
        printf("no cgroup mounted");
        exit(1);
    }
}

void add_to_cgroup(pid_t pid, const char CGROUP_ID[20]) {
    int _cversion = cgroup_verison(CGROUP_PATH);
    char _spid[20];
    char __path[100];
    sprintf(_spid, "%i", pid);
    switch (_cversion) {
    case 2: {
        sprintf(__path, "%s/%s/cgroup.procs", MAIN_CGROUP_PATH, CGROUP_ID);
        chmod(__path, 0777);
        echo_to_file(__path, _spid, strlen(_spid));
        break;
    }
    case 1: {
        sprintf(__path, "%s/cpuset/yats/%s/tasks", CGROUP_PATH, CGROUP_ID);
        echo_to_file(__path, _spid, strlen(_spid));
        sprintf(__path, "%s/cpu/yats/%s/tasks", CGROUP_PATH, CGROUP_ID);
        echo_to_file(__path, _spid, strlen(_spid));
        sprintf(__path, "%s/memory/yats/%s/tasks", CGROUP_PATH, CGROUP_ID);
        echo_to_file(__path, _spid, strlen(_spid));
        break;
    }
    case 0: // no cgroup mounted (error)
        printf("no cgroup mounted");
        exit(1);
    }
}

void deinit_cgroup(const char CGROUP_ID[20]) {
    char __path[100];
    sprintf(__path, "%s/%s", MAIN_CGROUP_PATH, CGROUP_ID);
    SAFE("deinit err", rmdir(__path));
}

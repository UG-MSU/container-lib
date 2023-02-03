#include "container-lib/cgroups.hpp"

void echo_to_file(const char *path, const char *text, int len) {
    int fd;
    SAFE("file open error", fd = open(path, O_WRONLY));
    if(len != write(fd, text, len)) {
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
    stringstream s;
    _str_cpu = rand_cpu;
    s << (uint64_t)(1000000 * TOTAL_CPU_PERCENTAGE)<< '\a' << 1000000;
    _cpumax = s.str();
    s.clear();
    _memorymax = MEM_SIZE*1024;
    switch (_cversion) {
    case 2: { // cgroup v2
        s << MAIN_CGROUP_PATH << '\a' << CGROUP_ID;
        _cgroup = s.str();
        s.clear();
        mkdir(MAIN_CGROUP_PATH, 0777);
        SAFE("mkdir err:", mkdir(_cgroup, 0777));
        chmod(_cgroup, 0777);
        char _subtreepath[100];
        char __subtreepath[100];
        s << MAIN_CGROUP_PATH << "/cgroup.subtree_control";
        _subtreepath = s.str();
        s.clear();
        s << CGROUP_PATH << "/cgroup.subtree_control";
        _subtreepath = s.str();
        s.clear();
        echo_to_file(__subtreepath, "+cpu +cpuset", 12);
        echo_to_file(_subtreepath, "+cpu +memory +cpuset",
                     20); // enable controllers
        s << _cgroup << "/cpuset.cpus";
        __path = s.str();
        s.clear();
        echo_to_file(__path, _str_cpu, strlen(_str_cpu)); // set random cpu core
        s << _cgroup << "/cpu.max";
        __path = s.str();
        s.clear();
        echo_to_file(__path, _cpumax, strlen(_cpumax)); // set max cpu time
        s << _cgroup << "/memory.swap.max";
        __path = s.str();
        s.clear();
        echo_to_file(__path, "0", 1); //  disable memory swap
        s << _cgroup << "/memory.max";
        __path = s.str();
        s.clear();
        echo_to_file(__path, _memorymax, strlen(_memorymax)); // set max memory
        break;
    }
    case 1: { // cgroupv1
        sprintf(_cgroup, "%s/cpuset/yats", CGROUP_PATH);
        s << CGROUP_PATH << "/cpuset/yats";
        _cgroup = s.str();
        s.clear();
        mkdir(_cgroup, 0700);
        s << CGROUP_PATH << "/cpuset/yats/" << CGROUP_ID;
        _cgroup = s.str();
        s.clear();
        mkdir(_cgroup, 0700);
        s << _cgroup << "/cpuset.cpus";
        __path = s.str();
        s.clear();
        echo_to_file(__path, _str_cpu, strlen(_str_cpu)); // set random cpu core
        s << CGROUP_PATH << "/cpus/yats";
        _cgroup = s.str();
        s.clear();
        mkdir(_cgroup, 0700);
        s << CGROUP_PATH << "/cpu/yats/" << CGROUP_ID;
        _cgroup = s.str();
        s.clear();
        mkdir(_cgroup, 0700);
        s << _cgroup << "/cpu.max";
        __path = s.str();
        s.clear();
        echo_to_file(__path, _cpumax, strlen(_cpumax)); // set max cpu time
        s << CGROUP_PATH << "/memory/yats";
        _cgroup = s.str();
        s.clear();
        mkdir(_cgroup, 0700);
        //sprintf(_cgroup, "%s/memory/yats/%s", CGROUP_PATH, CGROUP_ID);
        s << CGROUP_PATH << "/memory/yats/" << CGROUP_ID;
        _cgroup = s.str();
        s.clear();
        mkdir(_cgroup, 0700);
        s << _cgroup << "/memory.swap.max";
        __path = s.str();
        s.clear();
        echo_to_file(__path, "0", 1); //  disable memory swap
        s << _cgroup << "/memory.max";
        __path = s.str();
        s.clear();
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
    stringstream s;
    s << pid;
    _spid = s.str();
    s.clear();
    switch (_cversion) {
    case 2: {
        s << MAIN_CGROUP_PATH << "/" << CGROUP_ID << "/cgroup.procs";
        __path = s.str();
        s.clear();
        chmod(__path, 0777);
        echo_to_file(__path, _spid, strlen(_spid));
        break;
    }
    case 1: {
        s << CGROUP_PATH << "/cpuset/yats/" << CGROUP_ID << "/tasks";
        __path = s.str();
        s.clear();
        echo_to_file(__path, _spid, strlen(_spid));
        s << CGROUP_PATH << "/cpu/yats/" << CGROUP_ID << "/tasks";
        __path = s.str();
        s.clear();
        echo_to_file(__path, _spid, strlen(_spid));
        s << CGROUP_PATH << "/memory/yats/" << CGROUP_ID << "/tasks";
        __path = s.str();
        s.clear();
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
    s << MAIN_CGROUP_PATH << "/" << CGROUP_ID;
    __path = s.str();
    s.clear();
    SAFE("deinit err", rmdir(__path));
}

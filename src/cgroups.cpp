#include "container-lib/cgroups.hpp"

void echo_to_file(std::string path, std::string text) {
    std::ofstream fs;
    std::cerr << text << std::endl;
    SAFE_FS(fs, path);
    if(fs.is_open()) std::cout << "open\n";
    else std::cout << "not open\n";
    std::cout << std::boolalpha << fs.is_open() << std::endl;
    fs << text;
    std::cerr << "writed to path: " << text << " path: " << path << "\n";
    fs.close();
}
// void echo_to_file(std::string path, std::string text) {
//     int fd;
//     const char * _path;
//     _path = path.c_str();
//     int len = text.size();
//     // std::cerr << len << '\n';
//     // std::cerr << text << '\n';
//     SAFE("file open error", fd = open(_path, O_WRONLY));
//     if(len != write(fd, text.c_str(), len)) {
//         perror("write error");
//     }
//     SAFE("file close error", close(fd));
// }
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
    std::string _cgroup;
    std::string _str_cpu;
    std::string _cpumax;
    std::string _memorymax;
    std::stringstream s;
    _str_cpu = rand_cpu;
    s << (uint64_t)(1000000 * TOTAL_CPU_PERCENTAGE) << ' ' << 1000000;
    _cpumax = s.str().c_str();
    s.str("");
    _memorymax = MEM_SIZE * 1024;
    switch (_cversion) {
    case 2: { // cgroup v2
        if (!FILE_EXISTS(MAIN_CGROUP_PATH))
            SAFE("mkdir err:", mkdir(MAIN_CGROUP_PATH, 0777));
        s << MAIN_CGROUP_PATH << '/' << CGROUP_ID;
        _cgroup = s.str().c_str();
        std::cerr << s.str() << '\n';
        echo_to_file("test.txt", s.str());
        s.str("");
        // std::cerr << _cgroup.c_str() << std::endl;
        if (!FILE_EXISTS(_cgroup.c_str())) {
            SAFE("mkdir err:", mkdir(_cgroup.c_str(), 0777));
            std::cerr << "cgroup not exists\n";
        } else {
            SAFE("rmdir err:", rmdir(_cgroup.c_str()));
            SAFE("mkdir err:", mkdir(_cgroup.c_str(), 0777));
            std::cerr << "cgroup exists\n";
        }
        // SAFE("mkdir err:", mkdir(_cgroup.c_str(), 0777));
        chmod(_cgroup.c_str(), 0777);
        s << MAIN_CGROUP_PATH << "/cgroup.subtree_control";
        echo_to_file(s.str(), "+cpu +memory +cpuset"); // enable controllers
        s.str("");
        s << CGROUP_PATH << "/cgroup.subtree_control";
        echo_to_file(s.str(), "+cpu +cpuset");
        s.str("");
        s << _cgroup << "/cpuset.cpus";
        echo_to_file(s.str(), _str_cpu); // set random cpu core
        s.str("");
        s << _cgroup << "/cpu.max";
        echo_to_file(s.str(), _cpumax); // set max cpu time
        s.str("");
        s << _cgroup << "/memory.swap.max";
        echo_to_file(s.str(), "0"); //  disable memory swap
        s.str("");
        s << _cgroup << "/memory.max";
        echo_to_file(s.str(), _memorymax); // set max memory
        s.str("");
        break;
    }
    case 1: { // cgroupv1
        s << CGROUP_PATH << "/cpuset/yats";
        mkdir(s.str().c_str(), 0700);
        s.str("");
        s << CGROUP_PATH << "/cpuset/yats/" << CGROUP_ID;
        mkdir(s.str().c_str(), 0700);
        s.str("");
        s << _cgroup << "/cpuset.cpus";
        echo_to_file(s.str(), _str_cpu); // set random cpu core
        s.str("");
        s << CGROUP_PATH << "/cpus/yats";
        mkdir(s.str().c_str(), 0700);
        s.str("");
        s << CGROUP_PATH << "/cpu/yats/" << CGROUP_ID;
        mkdir(s.str().c_str(), 0700);
        s.str("");
        s << _cgroup << "/cpu.max";
        echo_to_file(s.str(), _cpumax); // set max cpu time
        s.str("");
        s << CGROUP_PATH << "/memory/yats";
        mkdir(s.str().c_str(), 0700);
        s.str("");
        s << CGROUP_PATH << "/memory/yats/" << CGROUP_ID;
        mkdir(s.str().c_str(), 0700);
        s.str("");
        s << _cgroup << "/memory.swap.max";
        echo_to_file(s.str(), "0"); //  disable memory swap
        s.str("");
        s << _cgroup << "/memory.max";
        echo_to_file(s.str(), _memorymax); // set max memory
        s.str("");
        break;
    }
    case 0: // no cgroup mounted (error)
        printf("no cgroup mounted");
        exit(1);
    }
}
void add_to_cgroup(pid_t pid, const char CGROUP_ID[20]) {
    int _cversion = cgroup_verison(CGROUP_PATH);
    std::string _spid;
    std::string __path;
    std::stringstream s;
    s << pid;
    _spid = s.str();
    s.str("");
    switch (_cversion) {
    case 2: {
        s << MAIN_CGROUP_PATH << "/" << CGROUP_ID << "/cgroup.procs";
        chmod(s.str().c_str(), 0777);
        echo_to_file(s.str(), _spid);
        s.str("");
        break;
    }
    case 1: {
        s << CGROUP_PATH << "/cpuset/yats/" << CGROUP_ID << "/tasks";
        echo_to_file(s.str(), _spid);
        s.str("");
        s << CGROUP_PATH << "/cpu/yats/" << CGROUP_ID << "/tasks";
        echo_to_file(s.str(), _spid);
        s.str("");
        s << CGROUP_PATH << "/memory/yats/" << CGROUP_ID << "/tasks";
        echo_to_file(s.str(), _spid);
        s.str("");
        break;
    }
    case 0: // no cgroup mounted (error)
        printf("no cgroup mounted");
        exit(1);
    }
}

void deinit_cgroup(const char CGROUP_ID[20]) {
    std::string __path;
    std::stringstream s;
    s << MAIN_CGROUP_PATH << "/" << CGROUP_ID;
    SAFE("deinit err", rmdir(s.str().c_str()));
    s.str("");
}

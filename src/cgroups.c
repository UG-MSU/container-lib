#include <errno.h>
#include <fcntl.h>
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
#define safe(func, call)                                                       \
    if ((call) < 0) {                                                          \
        perror(func);                                                          \
        exit(1);                                                               \
    }

const char CGROUP_PATH[20] = "/sys/fs/cgroup";
const char MAIN_CGROUP_PATH[20] = "/sys/fs/cgroup/yats";
void echo_to_file(const char *path, const char *text, int len) {
    int fd;
    safe("file open error", fd = open(path, O_WRONLY));
    write(fd, text, len); // throws error but works
    safe("file close error", close(fd));
}
int cgroup_verison(const char CGROUP_PATH[50]) {
    struct statfs _buf;
    safe("statfs error", statfs(CGROUP_PATH, &_buf));
    long long fs_type = _buf.f_type;
    if (fs_type == 1667723888) // v2
        return 2;
    if (fs_type == 2613483) // v1
        return 1;
    else
        mount("cgroup2", CGROUP_PATH, "cgroup2", 0, NULL);
    return 2; // not a cgroup fs
}
void init_cgroup(long long MEM_SIZE, float TOTAL_CPU_PERCENTAGE,
                 char CGROUP_ID[20]) {
    const int cpu_n = get_nprocs();
    const int rand_cpu = rand() % cpu_n;
    int _cversion = cgroup_verison(CGROUP_PATH);
    char _cgroup[100];
    char __path[100];
    char _str_cpu[3];
    char _cpumax[30];
    char _memorymax[30];
    sprintf(_str_cpu, "%i", rand_cpu);
    sprintf(_cpumax, "%lli %lli", (int)(1000000 * TOTAL_CPU_PERCENTAGE),
            1000000);
    sprintf(_memorymax, "%lli", MEM_SIZE * 1024);
    switch (_cversion) {
    case 2: { // cgroup v2
        sprintf(_cgroup, "%s/%s", MAIN_CGROUP_PATH, CGROUP_ID);
        mkdir(MAIN_CGROUP_PATH, 0777);
        safe("mkdir err:", mkdir(_cgroup, 0777));
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
        sprintf(_cgroup, "%s/cpuset/%s", CGROUP_PATH, CGROUP_ID);
        mkdir(_cgroup, 0777);
        sprintf(__path, "%s/cpuset.cpus", _cgroup);
        echo_to_file(__path, _str_cpu, strlen(_str_cpu)); // set random cpu core
        sprintf(_cgroup, "%s/cpu/%s", CGROUP_PATH, CGROUP_ID);
        mkdir(_cgroup, 0777);
        sprintf(__path, "%s/cpu.max", _cgroup);
        echo_to_file(__path, _cpumax, strlen(_cpumax)); // set max cpu time
        sprintf(_cgroup, "%s/memory/%s", CGROUP_PATH, CGROUP_ID);
        mkdir(_cgroup, 0777);
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
void add_to_cgroup(pid_t pid, char CGROUP_ID[20]) {
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
        sprintf(__path, "%s/cpuset/%s/tasks", CGROUP_PATH, CGROUP_ID);
        echo_to_file(__path, _spid, strlen(_spid));
        sprintf(__path, "%s/cpu/%s/tasks", CGROUP_PATH, CGROUP_ID);
        echo_to_file(__path, _spid, strlen(_spid));
        sprintf(__path, "%s/memory/%s/tasks", CGROUP_PATH, CGROUP_ID);
        echo_to_file(__path, _spid, strlen(_spid));
        break;
    }
    case 0: // no cgroup mounted (error)
        printf("no cgroup mounted");
        exit(1);
    }
}

void deinit_cgroup(char CGROUP_ID[20]) {
    char __path[100];
    sprintf(__path, "%s/%s", MAIN_CGROUP_PATH, CGROUP_ID);
    safe("deinit err", rmdir(__path));
}
int main(int argc, char **argv) {
    srand(time(0));
    init_cgroup(500000, 0.7, "test_cgroup2"); // 5000000 10
    printf("cgroup init\n");
    pid_t pid = fork();
    if (pid == 0) {
        execl("./test", "");
    } else {
        printf("i'm parent, child pid: %i\n", pid);
        int status;
        add_to_cgroup(pid, "test_cgroup2");
        wait(&status);
        printf("child ended with %i\n", status);
        deinit_cgroup("test_cgroup2");
    }
    return 0;
}

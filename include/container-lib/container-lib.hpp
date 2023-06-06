#ifndef CONTAINER_LIB_HPP
#define CONTAINER_LIB_HPP
#include <asm/unistd.h>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <fcntl.h>
#include <iostream>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <random>
#include <signal.h>
#include <ctime>


#include <sys/mman.h>
namespace ContainerLib {

class Container {
protected:
    std::string buf;
public:
    using time_t = size_t;
    using fd_t = int;
    struct launch_options {
        time_t time;
        size_t forks_threshold;
        std::string input;
        uint64_t memory;
        std::string cgroup_id;
        double cpu_usage;
    };

    enum class ExitStatus {
        ok,
        compilation_error,
        wrong_answer,
        presentation_error,
        time_limit_exceeded,
        memory_limit_exceeded,
        output_limit_exceeded,
        run_time_error,
        precompile_check_failed,
        idleness_limit_exceeded
    };

    enum class Syscall {
        kill,
        fork,
        clone,
        vfork,
        execve,
        mkdir,
        rmdir,
        reboot,
        open,
        openat,
        sethostname,
        setdomainname,
        creat,
        connect
    };
    virtual void start(std::string path_to_binary, launch_options options,
                       std::string args, std::set<Syscall> forbidden_syscalls) = 0;
    virtual ExitStatus sync(std::string cgroup_id) = 0;
    std::string get_buf() const;
};

class ContainerPipes : public Container {
    pid_t ptrace_proc, slave_proc;
    fd_t ptrace2exec[2], exec2ptrace[2], pipe_for_exit_status[2],
        ptrace2main[2];

    void ptrace_process(launch_options options,
                        std::set<Syscall> forbidden_syscalls);
    void create_processes(std::string path_to_binary, std::string args,
                          launch_options options,
                          std::set<Syscall> forbidden_syscalls);
    void pipe_init();
    void get_output(const fd_t *fd);
    void write_to_fd(const fd_t *fd, const char *string, size_t size);
    void kill_in_syscall(pid_t pid, user_regs_struct &state);

  public:
    void start(std::string path_to_binary, launch_options options,
               std::string args, std::set<Syscall> forbidden_syscalls) override;
    ExitStatus sync(std::string cgroup_id) override;
};
} // namespace ContainerLib
#endif

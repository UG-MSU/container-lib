#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace ContainerLib {
class Container {
  public:
    using time_t = size_t;
    using fd_t = int;
    struct launch_options {
        time_t time;
        size_t forks_amount;
        size_t forks_threshold;
    };

    enum class exit_status {
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

  private:
    pid_t main_proc, slave_proc;
    fd_t ptrace2exec[2], exec2ptrace[2], pipe_for_exit_status[2];
    std::string buf;
    void ptrace_process(launch_options options) const;
    void create_processes(std::string path_to_binary, std::string args,
                          launch_options options);
    void pipe_init();
    void get_output();

  public:
    void start(std::string path_to_binary, launch_options options,
               std::string args);
    exit_status sync() const;
    std::string get_buf() const;
};
} // namespace ContainerLib

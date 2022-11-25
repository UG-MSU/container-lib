#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>

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

  private:
    pid_t main_proc, slave_proc;
    fd_t ptrace2exec[2], exec2ptrace[2], ptrace2main[2];
    std::string buf;
    void ptrace_process(launch_options options);
    void create_processes(std::string path_to_binary, std::string args,
                          launch_options options);
    void pipe_init();
    void get_output(fd_t *fd);

  public:
    void start(std::string path_to_binary, launch_options options,
               std::string args);
    bool sync();
    std::string get_buf() const;
};
} // namespace ContainerLib

#include <iostream>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


namespace ContainerLib {
class Container {

public:
    using time_t = size_t;
    struct launch_options {
        time_t time;
        size_t forks_amount;
        size_t forks_threshold;
    };


private:
    pid_t main_proc, slave_proc;
    void ptrace_process(launch_options options) const;
    void create_processes(std::string path_to_binary, std::string args, launch_options options);

public:
    void start(std::string path_to_binary, launch_options options, std::string args);
    bool sync() const;
};
}

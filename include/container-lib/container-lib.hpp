#include <iostream>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

namespace ContainerLib {
    using time_t = size_t;

    struct launch_options {
        time_t time;
        size_t forks_amount;
        size_t forks_threshold;
    };

    bool synchronize(pid_t pid);

    pid_t start(std::string path_to_binary, launch_options options);
}
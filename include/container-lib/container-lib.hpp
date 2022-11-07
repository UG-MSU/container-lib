#include <iostream>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

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
        pid_t pid;
        void main_process(launch_options options) const;

    public:
        void start(std::string path_to_binary, launch_options options);
        bool synchronize() const;
    };
}

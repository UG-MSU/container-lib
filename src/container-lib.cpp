#include <iostream>
#include "../include/container_lib/cont_lib.h"

bool synchronize(pid_t pid) {
    return waitpid(pid, nullptr, 0);
}

void main_process(pid_t pid, ContainerLib::launch_options) {
    int status;
    do {
        ptrace(PTRACE_SYSCALL, pid, nullptr, nullptr);
        waitpid(pid, &status, 0);
        std::cout << status << std::endl;
    }
    while (WIFEXITED(status));
}

pid_t start(std::string path_to_binary, ContainerLib::launch_options options) {
    pid_t pid = fork();
    if (pid) {
        pid_t pid1 = fork();
        if (pid1) {
            return pid1;
        }
        else {
            main_process(pid, options);
        }
    }
}
else {
ptrace(PTRACE_TRACEME, 0, 0, 0);
execl(path_to_binary.data(), nullptr);
perror("execl");
}
return pid;
}
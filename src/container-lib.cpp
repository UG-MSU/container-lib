#include <iostream>
#include "../include/container-lib/container-lib.hpp"

void ContainerLib::Container::main_process(launch_options) const {
    int status;
    do {
        ptrace(PTRACE_SYSCALL, pid, nullptr, nullptr);
        waitpid(pid, &status, 0);
        std::cout << status << std::endl;
    }
    while (WIFEXITED(status));
}


void ContainerLib::Container::start(std::string path_to_binary, launch_options options) {
    pid = fork();
    if (pid) {
        pid_t pid1 = fork();
        if (pid1) {
            return;
        }
        else {
            main_process(options);
        }
    }
    else {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl(path_to_binary.data(), nullptr);
        perror("execl");
    }
}

bool ContainerLib::Container::synchronize() const {
    return waitpid(pid, nullptr, 0);
}

#include <iostream>
#include "container-lib/container-lib.hpp"

void ContainerLib::Container::main_process(launch_options) const {
    int status;
    do {
        ptrace(PTRACE_SYSCALL, slave_proc, nullptr, nullptr);
        waitpid(slave_proc, &status, 0);
        std::cout << status << std::endl;
    }
    while (WIFEXITED(status));
}


void ContainerLib::Container::start(std::string path_to_binary, launch_options options) {
    main_proc = fork();
    if (main_proc) {
        slave_proc = fork();
        if (slave_proc) {
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
    return waitpid(main_proc, nullptr, 0);
}

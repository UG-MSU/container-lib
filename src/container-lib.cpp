#include <iostream>
#include "container-lib/container-lib.hpp"

void ContainerLib::Container::main_process(launch_options options) const {
    int status;
    do {
        ptrace(PTRACE_SYSCALL, slave_proc, nullptr, nullptr);
        waitpid(slave_proc, &status, 0);
        std::cout << status << std::endl;
    }
    while (WIFEXITED(status));
}


void ContainerLib::Container::start(std::string path_to_binary, launch_options options, std::string args) {
    main_proc = fork();
    if (main_proc) {
        return;
    }
    else {
        slave_proc = fork();
        if (slave_proc) {
            main_process(options);
        }
        else {
            ptrace(PTRACE_TRACEME, 0, 0, 0);
            execl(path_to_binary.data(), args.data());
            perror("execl");
        }
    }
}

bool ContainerLib::Container::sync() const {
    return waitpid(main_proc, nullptr, 0);
}

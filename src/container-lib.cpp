#include <iostream>
#include "container-lib/container-lib.hpp"

void ContainerLib::Container::ptrace_process(launch_options options) const {
    int status;
    do {
        ptrace(PTRACE_SYSCALL, slave_proc, nullptr, nullptr);
        waitpid(slave_proc, &status, 0);
        std::cout << status << std::endl;
    } while (WIFEXITED(status));
}

void ContainerLib::Container::start(std::string path_to_binary,
                                    launch_options options, std::string args) {
    main_proc = fork();
    if (main_proc != 0) {
        return;
    } else {
        create_processes(std::move(path_to_binary), std::move(args),
                         std::move(options));
    }
}

bool ContainerLib::Container::sync() const {
    return waitpid(main_proc, nullptr, 0);
}

void ContainerLib::Container::create_processes(
    std::string path_to_binary, std::string args,
    ContainerLib::Container::launch_options options) {
    int streams[2];
    slave_proc = fork();
    if (slave_proc != 0) {
        ptrace_process(options);
    } else {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl(path_to_binary.data(), args.data(), nullptr);
        perror("execl");
    }
}

#include "container-lib/container-lib.hpp"

namespace ContainerLib {
void Container::main_process(launch_options) const {
    int status;
    do {
        ptrace(PTRACE_SYSCALL, slave_proc, nullptr, nullptr);
        waitpid(slave_proc, &status, 0);
        std::cout << status << std::endl;
    } while (WIFEXITED(status));
}

void Container::create_processes() {
    main_proc = fork();
    if (getpid() != main_proc)
        slave_proc = fork();
    return;
}

void Container::start(std::string path_to_binary, launch_options options) {
    create_processes();
    if (getpid() != main_proc && getpid() != slave_proc)
        main_process(options);
    else {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl(path_to_binary.data(), nullptr);
        perror("execl");
    }
}

bool Container::sync() const {
    if (getpid() != main_proc && getpid() != slave_proc) {
        wait(nullptr);
        wait(nullptr);
    }
    return 1; // the stub, have to change
}

} // namespace ContainerLib

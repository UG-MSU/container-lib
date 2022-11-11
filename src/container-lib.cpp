#include "container-lib/container-lib.hpp"

void ContainerLib::Container::ptrace_process(launch_options options) const {
    int status;
    waitpid(slave_proc, &status, 0);

    ptrace(PTRACE_SETOPTIONS, slave_proc, 0, PTRACE_O_TRACESYSGOOD);
    while (!WIFEXITED(status)) {

        struct user_regs_struct state;

        ptrace(PTRACE_SYSCALL, slave_proc, 0, 0);
        waitpid(slave_proc, &status, 0);

        // at syscall
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            ptrace(PTRACE_GETREGS, slave_proc, 0, &state);
            std::cout << "SYSCALL " << state.orig_rax << " at " << state.rip
                      << std::endl;

            // skip after syscall
            ptrace(PTRACE_SYSCALL, slave_proc, 0, 0);
            waitpid(slave_proc, &status, 0);
        }
    }
}

void ContainerLib::Container::start(std::string path_to_binary,
                                    launch_options options, std::string args) {
    pipe_init();
    main_proc = fork();
    if (main_proc != 0) {
        return;
    } else {
        create_processes(std::move(path_to_binary), std::move(args), options);
    }
}

bool ContainerLib::Container::sync() const {
    return waitpid(main_proc, nullptr, 0);
}

void ContainerLib::Container::create_processes(
    std::string path_to_binary, std::string args,
    ContainerLib::Container::launch_options options) {
    slave_proc = fork();
    if (slave_proc != 0) {
        ptrace_process(options);
    } else {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        dup2(fd_2[0], STDIN_FILENO);
        dup2(fd_1[1], STDOUT_FILENO);
        execl(path_to_binary.data(), args.data(), nullptr);
        perror("execl");
    }
}

void ContainerLib::Container::pipe_init() {
    pipe(fd_1);
    pipe(fd_2);
}

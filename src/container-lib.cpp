#include "container-lib/container-lib.hpp"
#include <asm/unistd.h>
#include <cstring>
#include <sys/user.h>
#include <sys/wait.h>

void ContainerLib::Container::ptrace_process(launch_options options) {
    int status, exit_status;
    ;
    waitpid(slave_proc, &status, 0);

    ptrace(PTRACE_SETOPTIONS, slave_proc, 0, PTRACE_O_TRACESYSGOOD);

    while (!WIFEXITED(status)) {

        user_regs_struct state{};

        ptrace(PTRACE_SYSCALL, slave_proc, 0, 0);
        waitpid(slave_proc, &status, 0);
        
        // at syscall
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            ptrace(PTRACE_GETREGS, slave_proc, 0, &state);
            switch (state.orig_rax) {
            case __NR_clone:
                if (fork() == 0) {
                    ptrace(PTRACE_GETREGS, slave_proc, 0, &state);
                    slave_proc = state.rax;
                    ptrace(PTRACE_ATTACH, slave_proc, 0, 0);
                    waitpid(slave_proc, &status, 0);
                    ptrace(PTRACE_SETOPTIONS, slave_proc, 0,
                           PTRACE_O_TRACESYSGOOD);
                }
                break;
            case __NR_execve:
                state.rax = __NR_kill;
                state.rdi = slave_proc;
                state.rsi = SIGKILL;
                ptrace(PTRACE_SETREGS, slave_proc, 0, &state);
                ptrace(PTRACE_CONT, slave_proc, 0, 0);
                waitpid(slave_proc, nullptr, 0);
                std::cout << "Program broken because it called execv"
                          << std::endl;
                return;
            default:
                std::cout << "SYSCALL " << state.orig_rax << " at " << state.rip
                          << std::endl;
                break;
            }

            // skip after syscall
            ptrace(PTRACE_SYSCALL, slave_proc, 0, 0);
            waitpid(slave_proc, &status, 0);
        }
    }
    get_output(exec2ptrace);
    write_to_fd(ptrace2main, buf.c_str(), buf.size());
    exit(exit_status);
}

void ContainerLib::Container::start(std::string path_to_binary,
                                    launch_options options, std::string args) {
    pipe_init();
    ptrace_proc = fork();
    if (ptrace_proc != 0) {
        return;
    } else {
        create_processes(std::move(path_to_binary), std::move(args), options);
    }
}

bool ContainerLib::Container::sync() {
    int a = getpid();
    int status;
    if (slave_proc != 0 && ptrace_proc != 0)
        waitpid(ptrace_proc, &status, 0);
    if (WIFEXITED(status)) {
        get_output(ptrace2main);
        return 1;
    }
    return 0;
}

void ContainerLib::Container::create_processes(
    std::string path_to_binary, std::string args,
    ContainerLib::Container::launch_options options) {
    slave_proc = fork();
    if (slave_proc != 0) {
        ptrace_process(options);
    } else {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        write_to_fd(ptrace2exec, options.input.c_str(), options.input.size());
        dup2(ptrace2exec[0], STDIN_FILENO);
        dup2(exec2ptrace[1], STDOUT_FILENO);
        execl(path_to_binary.data(), args.data(), nullptr);
        perror("execl");
    }
}

void ContainerLib::Container::pipe_init() {
    pipe(ptrace2exec);
    pipe(exec2ptrace);
    pipe(ptrace2main);
    fcntl(exec2ptrace[0], F_SETFL, O_NONBLOCK);
    fcntl(ptrace2main[0], F_SETFL, O_NONBLOCK);
    fcntl(ptrace2exec[0], F_SETFL, O_NONBLOCK);
}

std::string ContainerLib::Container::get_buf() const { return buf; }

void ContainerLib::Container::get_output(const fd_t *fd) { // updates buf
    std::stringstream input;
    char tmp;
    int nread = 0;
    while (nread != -1) {
        nread = read(fd[0], &tmp, 1);
        input << tmp;
    }
    buf = input.str();
    return;
}

void ContainerLib::Container::write_to_fd(const fd_t *fd, const char *string,
                                          size_t size) {
    char *tmp = new char[size];
    strcpy(tmp, string);
    int write_status = write(fd[1], tmp, size);
    delete[] tmp;
    return;
}

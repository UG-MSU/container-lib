#include "container-lib/container-lib.hpp"

void ContainerLib::Container::ptrace_process(launch_options options) {
    int status, exit_status;
    waitpid(slave_proc, &status, 0);

    ptrace(PTRACE_SETOPTIONS, slave_proc, 0, PTRACE_O_TRACESYSGOOD);

    while (!WIFEXITED(status)) {

        user_regs_struct state{};

        ptrace(PTRACE_SYSCALL, slave_proc, 0, 0);
        waitpid(slave_proc, &status, 0);

        // at syscall
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            ptrace(PTRACE_GETREGS, slave_proc, 0, &state);
            ContainerLib::Container::exit_status return_status;
            switch (state.orig_rax) {
            case __NR_execve:
                std::cout << "process " << slave_proc
                          << " tried to execute a binary! killing process!"
                          << std::endl;
                state.rax = __NR_kill;
                state.rdi = slave_proc;
                state.rsi = SIGKILL;
                ptrace(PTRACE_SETREGS, slave_proc, 0, &state);
                ptrace(PTRACE_CONT, slave_proc, 0, 0);
                waitpid(slave_proc, nullptr, 0);
                return_status = exit_status::run_time_error;
                write(pipe_for_exit_status[1], &return_status,
                      sizeof(exit_status));
                return;
            case __NR_clone:
                std::cout << "process " << slave_proc
                          << " tried to clone itself! killing process!"
                          << std::endl;
                state.rax = __NR_kill;
                state.rdi = slave_proc;
                state.rsi = SIGKILL;
                ptrace(PTRACE_SETREGS, slave_proc, 0, &state);
                ptrace(PTRACE_CONT, slave_proc, 0, 0);
                return_status = exit_status::run_time_error;
                write(pipe_for_exit_status[1], &return_status,
                      sizeof(exit_status));
                return;
            case __NR_fork:
                std::cout << "Process with " << slave_proc << " pid forked to process with " << state.rax << " pid" << std::endl;
                if (fork() == 0) {
                    slave_proc = state.rax;
                    ptrace(PTRACE_ATTACH, slave_proc, 0, 0);
                    waitpid(slave_proc, &status, 0);
                    ptrace(PTRACE_SETOPTIONS, slave_proc, 0, PTRACE_O_TRACESYSGOOD);
                }
            default:
                std::cout << "SYSCALL " << state.orig_rax << " at " << state.rip
                          << std::endl;
            }

            // skip after syscall
            ptrace(PTRACE_SYSCALL, slave_proc, 0, 0);
            waitpid(slave_proc, &status, 0);
        }
    }
    ContainerLib::Container::exit_status return_status = exit_status::ok;
    write(pipe_for_exit_status[1], &return_status, sizeof(exit_status));
    get_output(exec2ptrace);
    write_to_fd(ptrace2main, buf.c_str(), buf.size());
    exit(exit_status);
}

void ContainerLib::Container::start(std::string path_to_binary, launch_options options, std::string args) {
    pipe_init();
    ptrace_proc = fork();
    if (ptrace_proc != 0) {
        close(pipe_for_exit_status[1]);
        return;
    } else {
        close(pipe_for_exit_status[0]);
        create_processes(std::move(path_to_binary), std::move(args), options);
    }
}

ContainerLib::Container::exit_status ContainerLib::Container::sync() {
    int ptrace_status;
    waitpid(ptrace_proc, &ptrace_status, 0);
    exit_status status;
    if (WIFEXITED(ptrace_status)) {
        read(pipe_for_exit_status[0], &status, sizeof(exit_status));
        get_output(ptrace2main);
        return status;
    }
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
    pipe(ptrace2main);
    pipe(exec2ptrace);
    pipe(pipe_for_exit_status);
    fcntl(exec2ptrace[0], F_SETFL, O_NONBLOCK);
    fcntl(ptrace2main[0], F_SETFL, O_NONBLOCK);
    fcntl(ptrace2exec[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_for_exit_status[0], F_SETFL, O_NONBLOCK);
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

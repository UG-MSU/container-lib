#include "container-lib/cgroups.hpp"
#include "container-lib/container-lib.hpp"
void ContainerLib::ContainerPipes::ptrace_process(
    launch_options options, std::set<Syscall> forbidden_syscalls) {
    std::time_t start_tl;
    int status;
    ContainerLib::SharedMemory<size_t> threads_amount("threadscnt");
    *(threads_amount.memory) = 1;
    bool time_limit_status = false;
    waitpid(slave_proc, &status, 0);

    ptrace(PTRACE_SETOPTIONS, slave_proc, 0, PTRACE_O_TRACESYSGOOD);
    start_tl = std::time(nullptr);
    SAFE("ctime error", start_tl);
    while (!WIFEXITED(status)) {
        user_regs_struct state{};
        ptrace(PTRACE_SYSCALL, slave_proc, 0, 0);
        waitpid(slave_proc, &status, 0);
        // at Syscall
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            if (*(threads_amount.memory) >= options.forks_threshold) {
                kill_in_syscall(slave_proc, state, ExitStatus::run_time_error);
                exit(0);
            }
            ptrace(PTRACE_GETREGS, slave_proc, 0, &state);
            if (std::time(nullptr) - start_tl >= options.time) {
                kill_in_syscall(slave_proc, state,
                                ExitStatus::time_limit_exceeded);
                std::cerr << "timelimit exceeded\n";
                exit(0);
            }
            switch (state.orig_rax) {
            case __NR_execve:
                if (forbidden_syscalls.count(Syscall::execve)) {

                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
            case __NR_clone:
                if (forbidden_syscalls.count(Syscall::clone)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                } else {
                    if (fork() == 0) {
                        ++*(threads_amount.memory);
                        slave_proc = state.rax;
                        ptrace(PTRACE_ATTACH, slave_proc, 0, 0);
                        waitpid(slave_proc, &status, 0);
                        ptrace(PTRACE_SETOPTIONS, slave_proc, 0,
                               PTRACE_O_TRACESYSGOOD);
                    }
                }
                break;
            case __NR_fork:
                if (forbidden_syscalls.count(Syscall::fork)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                } else {
                    if (fork() == 0) {
                        ++*(threads_amount.memory);
                        slave_proc = state.rax;
                        ptrace(PTRACE_ATTACH, slave_proc, 0, 0);
                        waitpid(slave_proc, &status, 0);
                        ptrace(PTRACE_SETOPTIONS, slave_proc, 0,
                               PTRACE_O_TRACESYSGOOD);
                    }
                }
                break;

            case __NR_kill:
                if (forbidden_syscalls.count(Syscall::kill)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_vfork:
                if (forbidden_syscalls.count(Syscall::vfork)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_mkdir:
                if (forbidden_syscalls.count(Syscall::mkdir)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_rmdir:
                if (forbidden_syscalls.count(Syscall::rmdir)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_reboot:
                if (forbidden_syscalls.count(Syscall::reboot)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_open:
                if (forbidden_syscalls.count(Syscall::open)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_openat:
                if (forbidden_syscalls.count(Syscall::openat)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_sethostname:
                if (forbidden_syscalls.count(Syscall::sethostname)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_setdomainname:
                if (forbidden_syscalls.count(Syscall::setdomainname)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_creat:
                if (forbidden_syscalls.count(Syscall::creat)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            case __NR_connect:
                if (forbidden_syscalls.count(Syscall::connect)) {
                    kill_in_syscall(slave_proc, state,
                                    ExitStatus::run_time_error);
                    exit(0);
                }
                break;
            }

            // skip after Syscall
            ptrace(PTRACE_SYSCALL, slave_proc, 0, 0);
            waitpid(slave_proc, &status, 0);
        }
    }
    ContainerLib::Container::ExitStatus return_status = ExitStatus::ok;
    SAFE("write error in ExitStatus",
         write(pipe_for_exit_status[1], &return_status, sizeof(return_status)));
    get_output(exec2ptrace);
    write_to_fd(ptrace2main, buf.c_str(), buf.size());
    shm_unlink("threadscnt");
    exit(0);
}

void ContainerLib::ContainerPipes::start(std::string path_to_binary,
                                         launch_options options,
                                         std::string args,
                                         std::set<Syscall> forbidden_syscalls) {

    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> uniform_dist(
        0, sysconf(_SC_NPROCESSORS_ONLN) - 1);
    int coreCPU = uniform_dist(e);
    cgroup.init(options.memory, options.cpu_usage, options.cgroup_id, coreCPU);
    pipe_init();
    ptrace_proc = fork();

    if (ptrace_proc != 0) {
        close(pipe_for_exit_status[1]);
        return;
    } else {

        close(pipe_for_exit_status[0]);
        create_processes(std::move(path_to_binary), std::move(args), options,
                         forbidden_syscalls);
    }
}

ContainerLib::Container::ExitStatus
ContainerLib::ContainerPipes::sync(std::string cgroup_id) {
    int ptrace_status;
    waitpid(ptrace_proc, &ptrace_status, 0);
    ExitStatus status;
    if (WIFEXITED(ptrace_status)) {
        SAFE("read error in sync",
             read(pipe_for_exit_status[0], &status, sizeof(ExitStatus)));
        get_output(ptrace2main);
        return status;
    }
}

void ContainerLib::ContainerPipes::create_processes(
    std::string path_to_binary, std::string args,
    ContainerLib::ContainerPipes::launch_options options,
    std::set<Syscall> forbidden_syscalls) {
    slave_proc = fork();
    if (slave_proc != 0) {
        ptrace_process(options, forbidden_syscalls);
    } else {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        write_to_fd(ptrace2exec, options.input.c_str(), options.input.size());
        dup2(ptrace2exec[0], STDIN_FILENO);
        dup2(exec2ptrace[1], STDOUT_FILENO);
        cgroup.add_process(getpid());
        execl(path_to_binary.data(), args.data(), nullptr);
        perror("execl");
    }
}

void ContainerLib::ContainerPipes::pipe_init() {
    SAFE("pipe error in ptrace2exec", pipe(ptrace2exec));
    SAFE("pipe error in ptrace2main", pipe(ptrace2main));
    SAFE("pipe error in exec2ptrace", pipe(exec2ptrace));
    SAFE("pipe error in pipe exit status", pipe(pipe_for_exit_status));
    fcntl(exec2ptrace[0], F_SETFL, O_NONBLOCK);
    fcntl(ptrace2main[0], F_SETFL, O_NONBLOCK);
    fcntl(ptrace2exec[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_for_exit_status[0], F_SETFL, O_NONBLOCK);
}

std::string ContainerLib::Container::get_buf() const { return buf; }

void ContainerLib::ContainerPipes::get_output(const fd_t *fd) { // updates buf
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

void ContainerLib::ContainerPipes::write_to_fd(const fd_t *fd,
                                               const char *string,
                                               size_t size) {
    char *tmp = new char[size];
    strcpy(tmp, string);
    int write_status = write(fd[1], tmp, size);
    delete[] tmp;
    return;
}
void ContainerLib::ContainerPipes::kill_in_syscall(pid_t pid,
                                                   user_regs_struct &state,
                                                   ExitStatus status) {
    state.orig_rax = __NR_kill;
    state.rdi = pid;
    state.rsi = SIGKILL;
    ptrace(PTRACE_SETREGS, pid, 0, &state);
    ptrace(PTRACE_CONT, pid, 0, 0);
    waitpid(pid, NULL, 0);
    SAFE("Write error in kill syscall",
         write(pipe_for_exit_status[1], &status, sizeof(status)));
}

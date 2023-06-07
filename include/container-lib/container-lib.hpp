#ifndef CONTAINER_LIB_HPP
#define CONTAINER_LIB_HPP
#include <asm/unistd.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <random>
#include <sys/mman.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

namespace fs = boost::filesystem;
using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;
namespace ContainerLib {

class Container {
protected:
    std::string buf;
public:
    using time_t = size_t;
    using fd_t = int;
    struct launch_options {
        time_t time;
        size_t forks_threshold;
        std::string input;
        uint64_t memory;
        std::string cgroup_id;
        double cpu_usage;
    };

    enum class ExitStatus {
        ok,
        compilation_error,
        wrong_answer,
        presentation_error,
        time_limit_exceeded,
        memory_limit_exceeded,
        output_limit_exceeded,
        run_time_error,
        precompile_check_failed,
        idleness_limit_exceeded
    };

    enum class Syscall {
        kill,
        fork,
        clone,
        vfork,
        execve,
        mkdir,
        rmdir,
        reboot,
        open,
        openat,
        sethostname,
        setdomainname,
        creat,
        connect
    };
    virtual void start(std::string path_to_binary, launch_options options,
                       std::string args, std::set<Syscall> forbidden_syscalls) = 0;
    virtual ExitStatus sync(std::string cgroup_id) = 0;
    std::string get_buf() const;
};

class ContainerPipes : public Container {
    pid_t ptrace_proc, slave_proc;
    fd_t ptrace2exec[2], exec2ptrace[2], pipe_for_exit_status[2],
        ptrace2main[2];

    void ptrace_process(launch_options options,
                        std::set<Syscall> forbidden_syscalls);
    void create_processes(std::string path_to_binary, std::string args,
                          launch_options options,
                          std::set<Syscall> forbidden_syscalls);
    void pipe_init();
    void get_output(const fd_t *fd);
    void write_to_fd(const fd_t *fd, const char *string, size_t size);
    void kill_in_syscall(pid_t pid, user_regs_struct &state);

  public:
    void start(std::string path_to_binary, launch_options options,
               std::string args, std::set<Syscall> forbidden_syscalls) override;
    ExitStatus sync(std::string cgroup_id) override;
};
} // namespace ContainerLib

class ContainerDocker : public Container{
class client {
private:
  io_service service;
  ip::tcp::endpoint ep;
  static size_t read_complete(char *buff, const error_code &err, size_t bytes);
  static std::string copy(std::string path2file);
  void sync_echo(std::string msg);
  std::string vec2str(std::vector<std::string> vec);
  std::string read_from_buff(ip::tcp::socket & sock);
  void send_file(ip::tcp::socket & sock);
  std::string file2string(std::string path2file);

public:
  client(int port = 8001, std::string local_add = "127.0.0.1");
  void start(std::vector<std::string> args, std::string path2file, 
             std::string path2input, launch_options options);
};
class server {
private:
  io_service service;
  ip::tcp::endpoint ep;
  static size_t read_complete(char *buff, const error_code &err, size_t bytes);
  std::string start_execution(std::string command);
  static std::string file2string(std::string path2file);
  void string2file(std::string output); 
  std::string read_from_buff(ip::tcp::socket & sock);
  std::string read_file_bytes(std::string name);

public:
  server(int port, std::string local_add = "127.0.0.1");
  void handle_connections();
};
class interpreted_test {
  private:
    client client;

  public:
    interpreted_test(int port, std::string dockerfile_path);
    
};
}
#endif

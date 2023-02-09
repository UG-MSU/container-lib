#include "../extern/pybind11/include/pybind11/pybind11.h"
#include "../extern/pybind11/include/pybind11/stl.h"
#include "container-lib/container-lib.hpp"
namespace py = pybind11;
using namespace std;
using namespace ContainerLib;

PYBIND11_MODULE(container_lib_py, m) {
    m.doc() = "containerization python lib";

    py::class_<Container> container(m, "Container");
    container.def(py::init<>())
        .def("sync", &Container::sync, "sync function")
        .def("start", &Container::start,
             "starts containerization process. Launch options struct: int time "
             "- start time, int forks_amount - Amount of sub-processes, int "
             "forks_threshold - Maximum amount of sub-processes")
         .def("get_buf", &Container::get_buf, "get buf");
        
    py::enum_<Container::ExitStatus>(container, "ExitStatus")
        .value("ok", Container::ExitStatus::ok)
        .value("compilation_error", Container::ExitStatus::compilation_error)
        .value("wrong_answer", Container::ExitStatus::wrong_answer)
        .value("presentation_error", Container::ExitStatus::presentation_error)
        .value("time_limit_exceeded",
               Container::ExitStatus::time_limit_exceeded)
        .value("memory_limit_exceeded",
               Container::ExitStatus::memory_limit_exceeded)
        .value("output_limit_exceeded",
               Container::ExitStatus::output_limit_exceeded)
        .value("run_time_error", Container::ExitStatus::run_time_error)
        .value("precompile_check_failed",
               Container::ExitStatus::precompile_check_failed)
        .value("idleness_limit_exceeded",
               Container::ExitStatus::idleness_limit_exceeded)
        .export_values();
    py::enum_<Container::Syscall>(container, "Syscall")
        .value("kill", Container::Syscall::kill)
        .value("fork", Container::Syscall::fork)
        .value("clone", Container::Syscall::clone)
        .value("vfork", Container::Syscall::vfork)
        .value("execve", Container::Syscall::execve)
        .value("mkdir", Container::Syscall::mkdir)
        .value("rmdir", Container::Syscall::rmdir)
        .value("reboot", Container::Syscall::reboot)
        .value("open", Container::Syscall::open)
        .value("openat", Container::Syscall::openat)
        .value("sethostname", Container::Syscall::sethostname)
        .value("setdomainname", Container::Syscall::setdomainname)
        .value("creat", Container::Syscall::creat)
        .value("connect", Container::Syscall::connect)
        .export_values();
    py::class_<Container::launch_options>(container, "launch_options",
                                          py::dynamic_attr())
        .def(py::init<>())
        .def_readwrite("time", &Container::launch_options::time)
        .def_readwrite("forks_amount", &Container::launch_options::forks_amount)
        .def_readwrite("forks_threshold",
                       &Container::launch_options::forks_threshold)
        .def_readwrite("cpu_usage", &Container::launch_options::cpu_usage)
        .def_readwrite("memory", &Container::launch_options::memory)
        .def_readwrite("cgroup_id", &Container::launch_options::cgroup_id)
        .def_readwrite("input", &Container::launch_options::input);

}

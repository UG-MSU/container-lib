#include "../extern/pybind11/include/pybind11/pybind11.h"
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
        
    py::enum_<Container::exit_status>(container, "exit_status")
        .value("ok", Container::exit_status::ok)
        .value("compilation_error", Container::exit_status::compilation_error)
        .value("wrong_answer", Container::exit_status::wrong_answer)
        .value("presentation_error", Container::exit_status::presentation_error)
        .value("time_limit_exceeded",
               Container::exit_status::time_limit_exceeded)
        .value("memory_limit_exceeded",
               Container::exit_status::memory_limit_exceeded)
        .value("output_limit_exceeded",
               Container::exit_status::output_limit_exceeded)
        .value("run_time_error", Container::exit_status::run_time_error)
        .value("precompile_check_failed",
               Container::exit_status::precompile_check_failed)
        .value("idleness_limit_exceeded",
               Container::exit_status::idleness_limit_exceeded)
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
        .def_readwrite("cgroup_id", &Container::launch_options::cgroup_id);
}

#include "../extern/pybind11/include/pybind11/pybind11.h"
#include "container-lib/container-lib.hpp"
#include "container-lib/exception.hpp"
namespace py = pybind11;
using namespace std;
using namespace ContainerLib;

PYBIND11_MODULE(container_lib_py, m) {
    m.doc() = "containerization python lib";
    py::class_<Exception>(m, "Exception")
        .def(py::init([](const char *arg) { return new Exception(arg); }))
        .def("what", &Exception::what, "Return exception error");

    py::class_<ContainerPipes> container(m, "ContainerPipes");
    container.def(py::init<>())
        .def("sync", &ContainerPipes::sync, "sync function")
        .def("start", &ContainerPipes::start,
             "starts containerization process. Launch options struct: int time "
             "- start time, int forks_amount - Amount of sub-processes, int "
             "forks_threshold - Maximum amount of sub-processes")
        .def("get_buf", &ContainerPipes::get_buf, "get buf");

    py::enum_<ContainerPipes::ExitStatus>(container, "ExitStatus")
        .value("ok", ContainerPipes::ExitStatus::ok)
        .value("compilation_error",
               ContainerPipes::ExitStatus::compilation_error)
        .value("wrong_answer", ContainerPipes::ExitStatus::wrong_answer)
        .value("presentation_error",
               ContainerPipes::ExitStatus::presentation_error)
        .value("time_limit_exceeded",
               ContainerPipes::ExitStatus::time_limit_exceeded)
        .value("memory_limit_exceeded",
               ContainerPipes::ExitStatus::memory_limit_exceeded)
        .value("output_limit_exceeded",
               ContainerPipes::ExitStatus::output_limit_exceeded)
        .value("run_time_error", ContainerPipes::ExitStatus::run_time_error)
        .value("precompile_check_failed",
               ContainerPipes::ExitStatus::precompile_check_failed)
        .value("idleness_limit_exceeded",
               ContainerPipes::ExitStatus::idleness_limit_exceeded)
        .export_values();
    py::class_<ContainerPipes::launch_options>(container, "launch_options",
                                               py::dynamic_attr())
        .def(py::init<>())
        .def_readwrite("time", &ContainerPipes::launch_options::time)
        .def_readwrite("forks_threshold",
                       &ContainerPipes::launch_options::forks_threshold)
        .def_readwrite("cpu_usage", &ContainerPipes::launch_options::cpu_usage)
        .def_readwrite("memory", &ContainerPipes::launch_options::memory)
        .def_readwrite("cgroup_id", &ContainerPipes::launch_options::cgroup_id)
        .def_readwrite("input", &ContainerPipes::launch_options::input);
}

#include "../extern/pybind11/include/pybind11/pybind11.h"
#include "container-lib/container-lib.hpp" 
namespace py = pybind11;
using namespace std;
using namespace ContainerLib;

PYBIND11_MODULE(container_lib_py, m) {
    m.doc() = "containerization python lib";
    py::class_<Container>(m, "Container")
        .def(py::init<>())
        .def("sync", &Container::sync, "sync function")
        .def("get_buf", &Container::get_buf, "get buffer")
        .def("start", &Container::start,
             "starts containerization process. Launch options struct: int time "
             "- start time, int forks_amount - Amount of sub-processes, int "
             "forks_threshold - Maximum amount of sub-processes");
    py::class_<Container::launch_options>(m, "launch_options",
                                          py::dynamic_attr())
        .def(py::init<>())
        .def_readwrite("time", &Container::launch_options::time)
        .def_readwrite("forks_amount", &Container::launch_options::forks_amount)
        .def_readwrite("forks_threshold",
                       &Container::launch_options::forks_threshold);
}


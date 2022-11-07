#include "../extern/pybind11/include/pybind11/pybind11.h"
#include "../include/container-lib/container-lib.hpp"
#include<string>
namespace py = pybind11;
using namespace std;


PYBIND11_MODULE(container_lib_py, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    py::class_<ContainerLib::launch_options>(m, "launch_options",py::dynamic_attr())
    .def(py::init<>())
    .def_readwrite("time", &ContainerLib::launch_options::time)
    .def_readwrite("forks_amount", &ContainerLib::launch_options::forks_amount)
    .def_readwrite("forks_threshold", &ContainerLib::launch_options::forks_threshold);
    m.def("create_processes", &ContainerLib::create_processes, "function that creates processes, returns PID of process");
    m.def("flow_control", &ContainerLib::flow_control, "bool value of flow control");
    m.def("start", &ContainerLib::start, "starts containerization process. Launch options struct: int time - start time, int forks_amount - Amount of sub-processes, int forks_threshold - Maximum amount of sub-processes");
}
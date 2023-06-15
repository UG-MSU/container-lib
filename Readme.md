# **ContainerLib** - _a YATS containerization library_

## About

This is the C++ containerization library, which is a part of YATS (Yet Another Testing System). It includes ContainerPipes and ContainerDocker classes, which can be used for different cases. Also, it includes bindings for python
## Quickstart

```console
foo@bar:~$ git submodule update --init --recursive
foo@bar:~$ mkdir build
foo@bar:~$ cd build
foo@bar:~$ cmake ..
foo@bar:~$ sudo make
```

## Example of code

```python
from container_lib_py import ContainerPipes, ContainerException

cont = ContainerPipes()
opt = cont.launch_options()

opt.time = 1000
opt.forks_threshold = 8
opt.cpu_usage = 0.9
opt.memory = 32
opt.cgroup_id = "container-lib"
opt.input = "foobar"

try:
    cont.start("./test", opt, "", set())
    exit_status = cont.sync()
    print(cont.get_buf())
    print(exit_status)
except ContainerException as e:
    print("Error!")
    # some handling here
    print(e)

```

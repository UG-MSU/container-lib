# **YATS** - _Yet Another Testing System_

system that smells _bebra_ and gives rating for it
![alt text](https://media.istockphoto.com/id/182203609/ru/%D1%84%D0%BE%D1%82%D0%BE/%D0%BF%D0%B0%D0%BB%D0%B5%D1%86-%D0%B2%D0%B2%D0%B5%D1%80%D1%85.jpg?s=1024x1024&w=is&k=20&c=f1IwGeHj3E52Cmif8WhdDxL2qcnGGaJvQARQ43RiMIo=) testing jew

## Features

- our project :)
- blackjack
- sluts
- report forks count
  
## WIP

- cgroups in ```container_lib.cpp```
- recursion limit
- sys_call blacklist
- container security
- Calculate working time and memory usage

## Ideas

- CTF
- just use docker
- github linkage

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

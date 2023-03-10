# **YATS** - _Yet Another Testing System_

system that smells *bebra* and gives rating for it
![alt text](https://media.istockphoto.com/id/182203609/ru/%D1%84%D0%BE%D1%82%D0%BE/%D0%BF%D0%B0%D0%BB%D0%B5%D1%86-%D0%B2%D0%B2%D0%B5%D1%80%D1%85.jpg?s=1024x1024&w=is&k=20&c=f1IwGeHj3E52Cmif8WhdDxL2qcnGGaJvQARQ43RiMIo=) testing jew 

## Dependences
- [pybind11](https://github.com/pybind/pybind11)
- libcurl4-openssl-dev
- [Docker-Client](https://github.com/piotr-maker/DockerClient/tree/8af68f7b5d2283f766b3c2e8161ae423ec105aa3)

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

## Example of code
```
# importing our library
import container_lib_py

# construct neccesary objects
cont = container_lib_py.Container()
opt = container_lib_py.launch_options()

# max time of executing program
opt.time = 1000

# max amount of processes executing program
opt.forks_threshold = 8

# percentage of processor load
opt.cpu_usage = 30

# max usage of memory KB
opt.memory = 32

opt.cgroup_id = "QWERTY

# input for controlled process
opt.input = "example of input"

# start binary file with way "./test", options opt and empty arguments
cont.start("./test", opt, "")
# YOU MUSTN'T CALL MORE THEN 1 METHOD START FOR COTAINER OBJECT!!!

# wait for end of the program and print its exit status
print(cont.sync())

# print output of the program
print(cont.get_buf())
```

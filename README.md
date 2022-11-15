# Container-lib

- A part of the YATS project

## Cloning 

This repo contains all necessary dependencies - including `pybind`. This meains that this repo must be cloned with submodules
```bash
git clone --recurse-submodules https://github.com/UG-MSU/container-lib.git
```

This library allso requires Python 3.6 or newer for bindings

## Building

This repo is build with CMake

```bash
mkdir build
cd build
cmake ..
make
```

Will compile both library and the python bindings.

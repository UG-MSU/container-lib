#include "container-lib/exception.hpp"

using namespace ContainerLib;

const char *ContainerException::what() const throw() { 
    return ContainerException::text.c_str(); 
}

#include "container-lib/exception.hpp"
#include <iostream>
using namespace ContainerLib;

const char *Exception::what() const throw() { 
    return Exception::text; 
}

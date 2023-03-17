// perror - проверка rno и exception
#include "container-lib/exception.hpp"
using namespace ContainerLib;
const char * Exception::what() const throw() {
    return Exception::text;
}

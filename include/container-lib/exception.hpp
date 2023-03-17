#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <exception>
#include <string>
namespace ContainerLib {

class Exception : public std::exception {
  public:
    const char *text;
    Exception(const char *func) : text(func) {}
    const char *what() const throw();
};
} // namespace ContainerLib
#endif

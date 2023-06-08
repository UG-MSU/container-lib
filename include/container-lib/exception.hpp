#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <exception>
#include <string>
namespace ContainerLib {

class Exception : public std::exception {
  public:
    std::string text;
    Exception(std::string func) : text(func) {}
    const char *what() const throw();
};
} // namespace ContainerLib
#endif

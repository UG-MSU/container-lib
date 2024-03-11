#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <exception>
#include <string>
namespace ContainerLib {

class ContainerException : public std::exception {
  public:
    std::string text;
    ContainerException(std::string func) : text(func) {}
    const char *what() const throw();
};
} // namespace ContainerLib
#endif

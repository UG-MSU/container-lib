#include <iostream>
#include <sys/wait.h>

int main() {
    std::cout << "Program works" << std::endl;
    execv("./test", nullptr);
}
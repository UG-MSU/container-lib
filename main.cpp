#include "container-lib/container-lib.hpp"
std::ostream &operator<<(std::ostream &os,
                         const ContainerLib::ContainerPipes::ExitStatus
                             &obj) { // just for get value from exit status enum
    os << static_cast<
        std::underlying_type<ContainerLib::ContainerPipes::ExitStatus>::type>(
        obj);
    return os;
}

int main(int argc, char *argv[]) {
    ContainerLib::ContainerPipes cont;
    cont.start(argv[1], {2, 10, "test", 50000, "container-lib", 0.9}, "", {});
    ContainerLib::ContainerPipes::ExitStatus es = cont.sync();
    std::string str = cont.get_buf();
    std::cout << std::endl << "OUTPUT:: " << str << std::endl;
    std::cout << "Exit status: " << es << std::endl;
}

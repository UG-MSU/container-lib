#include "container-lib/container-lib.hpp"
std::ostream &operator<<(std::ostream &os,
                         const ContainerLib::Container::ExitStatus
                             &obj) { // just for get value from exit status enum
    os << static_cast<
        std::underlying_type<ContainerLib::Container::ExitStatus>::type>(
        obj);
    return os;
}

int main(int argc, char *argv[]) {
    ContainerLib::Container cont;
    cont.start(argv[1], {2, 10, "test", 50000, "container-lib", 0.9}, "", {});
    ContainerLib::Container::ExitStatus es = cont.sync("container-lib");
    std::string str = cont.get_buf();
    std::cout << std::endl << "OUTPUT:: " << str << std::endl;
    std::cout << "Exit status: " << es << std::endl;
}

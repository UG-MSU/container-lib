#include "container-lib/container-lib.hpp"

int main(int argc, char *argv[]) {
    ContainerLib::ContainerPipes cont;
    cont.start(argv[1], {3, 0, 10, "test", 50000, "container-lib",0.8}, "", {});
    cont.sync("container-lib");
    std::string str = cont.get_buf();
    std::cout << std::endl << "OUTPUT:: " << str << std::endl;
}

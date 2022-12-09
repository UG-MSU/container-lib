#include "container-lib/container-lib.hpp"

int main(int argc, char *argv[]) {
    ContainerLib::Container cont;
    cont.start(argv[1], {0, 0, 10, "test", 50000, 0.1}, "");
    cont.sync();
    std::string str = cont.get_buf();
    std::cout << std::endl << "OUTPUT:: " << str << std::endl;
}

#include "container-lib/container-lib.hpp"

int main(int argc, char *argv[]) {
    ContainerLib::Container cont;
    cont.start(argv[1], {}, "");
    cont.sync();
}

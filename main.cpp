#include "container-lib/container-lib.hpp"
#include <iostream>

int main() {
    ContainerLib::Container cont;
    cont.start("/home/parat07/Documents/github/container-lib/test", {}, "");
    cont.sync();
}

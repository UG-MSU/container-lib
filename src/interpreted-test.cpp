#include "container-lib/container-lib.hpp"

ContainerLib::interpreted_test::interpreted_test(int port,
                                                 std::string dockerfile_path) {
    client(port);
}

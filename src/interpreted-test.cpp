#include "container-lib/container-lib.hpp"

ContainerLib::ContainerDocker::interpreted_test::interpreted_test(
    int port, std::string dockerfile_path) {
    ContainerLib::ContainerDocker::client::client(port);
}

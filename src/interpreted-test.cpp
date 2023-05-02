#include "interpreted-test/interpreted-test.hpp"

interpreted_test::interpreted_test(int port, std::string dockerfile_path) {
  client(port);
}
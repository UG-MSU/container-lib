#include "docker-lib/docker-lib.hpp"
#include <iostream>

using namespace std;

int main() {
  DockerLib::container cont;
  cont.init_container();
  return 0;   
}

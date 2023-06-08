#include "controle-prog/controle-prog.hpp"
#include <iostream>

int main() {
  server server(8000);
  server.handle_connections();
  return 0;
}
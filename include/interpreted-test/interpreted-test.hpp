#include "linking-server/linking-server.hpp"

class interpreted_test {
  private:
    client client;

  public:
    interpreted_test(int port, std::string dockerfile_path);
    
};
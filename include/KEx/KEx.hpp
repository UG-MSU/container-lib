#include <iostream>

class kex {
  pid_t main_th, slave_th;
  pid_t create_slave_stream();
  void flow_control();

  public:
    kex();

    void start();

    ~kex();
};

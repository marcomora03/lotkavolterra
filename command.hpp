#include <iostream>
#include <string>

#include "volterra.hpp"
namespace lv {
class command {
 private:
  char cmd_usr;
  Simulation& sim;
  int nsteps;
  double dt;
  void get_step_parameters();

 public:
  command(Simulation& sim_)
      : sim(sim_) {};  // costruttore, serve per applicare i metodi di command
                       // ad un simulation esistene
  void run_command();
};
}  // namespace lv

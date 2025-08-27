
#include "volterra.hpp"
namespace lv {
class command {
 private:
  char cmd_usr;
  Simulation& sim;
  int nsteps;
  double dt;

  void set_time();

 public:
  command(Simulation& sim_) : sim(sim_) {}
  void run_command();
};
}  

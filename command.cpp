#include "command.hpp"

#include <cmath>

#include "volterra.hpp"
using namespace lv;
void command::get_step_parameters() {
  double range = 0.001;

  do {
    std::cout << "inserire il numero di step\n";
    std::cin >> nsteps;
    if (nsteps <= 0) {
      std::cout << "inserire un valore maggiore di zero\n";
    }
  } while (nsteps <= 0);
  do {
    std::cout << "inserire la durata (dt) di ciascun step\n";
    std::cin >> dt;
    double rat = dt / range;
    if (rat < 1.0 || rat > 9.0) {
      std::cout << "inserire un numero dell ordine di 0.001\n";
    }
  } while (dt / range < 1.0 || dt / range > 9.0);
}

void command::run_command() {
  bool run_condition = true;
  while (run_condition) {
    std::cout << "inserire:" << std::endl
              << "'e' per iniziare una simulazione" << std::endl
              << "'f' per stampare la densità del numero di prede e predatori "
                 "e valore dell'integrale primo"
              << std::endl
              << "'p' per stampare la densità del numero di prede e predatori "
                 "e valore dell'integrale primo ad un dato step"
              << std::endl
              << "'f' per stampare la densità del numero di prede e predatori "
                 "e valore dell'integrale primo"
              << std::endl
              << "'a' per stampare la densità del numero di prede e predatori "
                 "e valore dell'integrale primo ad ogni step"
              << std::endl
              << "'c' per continuare la simulazione" << std::endl
              << "'q' terminare la simulazione" << std::endl;
    std::cin >> cmd_usr;
    if (cmd_usr == 'q') {
      std::cout << "termine simulazione";
      run_condition = false;
    } else  // guardare il perche degli else
      if (cmd_usr == 'e') {
        get_step_parameters();
        sim.e_evolve(dt, nsteps);
      } else if (cmd_usr == 'f') {
        const std::vector<Point>& result = sim.evolution_vector();
        std::cout << "il risultato della simulazione è\n";
        std::cout << result.back().x << "prede in vita\n";
        std::cout << result.back().y << "predatori in vita\n";
      } else if (cmd_usr == 'p') {
        int ncheck;
        do {
          std::cout
              << "inserire lo step del quale si vuole vedere lo stato della "
                 "simulazione\n";
          std::cin >> ncheck;
          if (ncheck < 0 ||
              static_cast<size_t>(ncheck) > sim.evolution_vector().size()) {
            std::cout << "inserire un numero di step valido\n";
          }
        } while (ncheck < 0 ||
                 static_cast<size_t>(ncheck) > sim.evolution_vector().size());
        Point state = sim.a_p_get_step_result(ncheck);
        std::cout << "al " << ncheck
                  << "-esimo step, il sistema si trovava nello stato\n";
        std::cout << state.x << " prede in vita\n";
        std::cout << state.y << " predatori in vita\n";
      } else if (cmd_usr == 'c') {
        get_step_parameters();
        sim.e_evolve(dt, nsteps);
      } else if (cmd_usr == 'a') {
        for (int i = 0; static_cast<size_t>(i) < sim.evolution_vector().size();
             i++) {
          Point state = sim.a_p_get_step_result(i);
          std::cout << "step " << i << " " << state.x << " prede " << state.y
                    << " predatori" << std::endl;
        }
      } else {
        std::cout << "Inserire comando valido\n";
      }
  }
}

#include "command.hpp"

#include <cmath>

#include "volterra.hpp"
using namespace lv;
void command::get_step_parameters() {
  const double range = 0.001;
  double ratio;
    std::cout << "inserire il numero di step\n";
    std::cin >> nsteps;
    if (std::cin.fail() ||nsteps <= 0) {
      std::cout << "Numero di step non valido\n";
      throw std::runtime_error("Numero di step non valido");}
    std::cout << "inserire la durata (dt) di ciascun step\n";
    std::cin >> dt;
    ratio = dt / range;
    if (ratio < 1.0 || ratio > 9.0) {
          throw std::runtime_error("inserire un numero dell ordine di 0.001\n");
    }}


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
        Point f_state = sim.a_p_get_step_result((sim.evolution_vector().size()-1));
        std::cout << "il risultato della simulazione è\n";
        std::cout << f_state.x << "prede in vita\n";
        std::cout << f_state.y << "predatori in vita\n";
        std::cout<< "l'integrale primo del moto vale "<<f_state.H<<std::endl;
      } else if (cmd_usr == 'p') {
        int ncheck;
          std::cout
       << "inserire lo step del quale si vuole vedere lo stato della "
                 "simulazione\n";
         std::cin >> ncheck;
        Point state = sim.a_p_get_step_result(ncheck);
        std::cout << "al " << ncheck
                  << "-esimo step, il sistema si trovava nello stato\n";
        std::cout << state.x << " prede in vita\n";
        std::cout << state.y << " predatori in vita\n";
        std::cout << state.H << " valore integrale primo\n";
      } else if (cmd_usr == 'c') {
        get_step_parameters();
        sim.e_evolve(dt, nsteps);
      } else if (cmd_usr == 'a') {
        for (int i = 0; static_cast<size_t>(i) < sim.evolution_vector().size();
             i++) {
          Point state = sim.a_p_get_step_result(i);
          std::cout << "step " << i << " " << state.x << " prede " << state.y
                    << " predatori" << " " << "H= "<<state.H<<std::endl;
        }
      } else {
         throw std::runtime_error("Comando inesistente");
      }
  }
}

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
      std::cout << "Numero di step n#include "command.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "volterra.hpp"
using namespace lv;
void command::set_time() {
  const double range = 0.001;
  double ratio;
  std::cout << "inserire il numero di step\n";
  std::cin >> nsteps;
  if (std::cin.fail() || nsteps <= 0) {
    std::cout << "Numero di step non valido\n";
    throw std::runtime_error("Numero di step non valido");
  }
  std::cout << "inserire la durata (dt) di ciascun step\n";
  std::cin >> dt;
  ratio = dt / range;
  if (std::cin.fail() || ratio < 1.0 || ratio > 9.0) {
    throw std::runtime_error("inserire un numero dell ordine di 10^-3\n");
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
              << "'a' per stampare la densità del numero di prede e predatori "
                 "e valore dell'integrale primo ad ogni step"
              << std::endl
              << "'c' per continuare la simulazione" << std::endl
              << "'q' terminare la simulazione" << std::endl;

    std::string input;
    std::getline(std::cin, input);
    if (input.size() != 1) {
      throw std::runtime_error("Inserire un solo carattere");
    }
    cmd_usr = input[0];
    if (cmd_usr == 'q') {
      std::cout << "Termine simulazione";
      run_condition = false;
    } else  // guardare il perche degli else
      if (cmd_usr == 'e') {
        if (sim.evolution_vector().size() == 1) {
          set_time();
          sim.evolve(dt, nsteps);
        } else {
          throw std::runtime_error("Simulazione già iniziata");
        }
      } else if (cmd_usr == 'f') {
        Point f_state =
            sim.get_step_result((sim.evolution_vector().size() - 1));
        std::cout << "il risultato della simulazione è\n";
        std::cout << f_state.x << " densità popolazione prede\n";
        std::cout << f_state.y << " densità popolazione predatori\n";
        std::cout << f_state.H << "valore integrale primo del moto"
                  << std::endl;
      } else if (cmd_usr == 'p') {
        size_t ncheck;
        std::cout
            << "inserire lo step del quale si vuole vedere lo stato della "
               "simulazione\n";
        std::cin >> ncheck;
        Point state = sim.get_step_result(ncheck);
        std::cout << "al " << ncheck
                  << "-esimo step, il sistema si trovava nello stato\n";
        std::cout << state.x << "  densità popolazione prede\n\n";
        std::cout << state.y << " densità popolazione predatori\n";
        std::cout << state.H << "valore integrale primo del moto \n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      } else if (cmd_usr == 'c') {
        if (sim.evolution_vector().size() > 1) {
          set_time();
          sim.evolve(dt, nsteps);
        } else {
          throw std::runtime_error(
              "Impossibile continuare simulazione non esistente");
        }
      } else if (cmd_usr == 'a') {
        for (size_t i = 0; i < sim.evolution_vector().size(); i++) {
          Point state = sim.get_step_result(i);
          std::cout << "step " << i << " " << state.x << " densità prede "
                    << state.y << " densità predatori" << " "
                    << "H= " << state.H << std::endl;
        }
      } else {
        throw std::runtime_error("Comando inesistente");
      }
  }
}
on valido\n";
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

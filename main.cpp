#include <iostream>
#include <vector>

#include "command.hpp"
#include "volterra.hpp"

int main() {
  std::cout << "inserire il valore dei parametri a b c d\n";
  auto params = lv::initialize_const();
  std::cout << "inserire il valore dei dati iniziali\n";
  auto point_i = lv::initialize_point();
  lv::Simulation prova1(point_i, params);
  lv::command prova1_run(prova1);
  prova1_run.run_command();
}

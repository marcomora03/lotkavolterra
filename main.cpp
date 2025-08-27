#include <iostream>
#include <vector>
#include <string>
#include "command.hpp"
#include "volterra.hpp"
#include <limits> 
int main() {
  try {
    lv::Constants params;
    lv::initialize_const(params);
    lv::Point point_i;
    set_zeropoint(point_i);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//per il buffer altrimenti rimane un \n visto dalle successive letture a schermo
    lv::Simulation prova1(point_i, params);
    lv::command prova1_run(prova1);
    prova1_run.run_command();
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << ". Uscita dal programma.\n";
    return 1;
  }
  return 0;
}

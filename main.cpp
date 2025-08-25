#include <iostream>
#include <vector>

#include "command.hpp"
#include "volterra.hpp"

int main() {
        try {
  lv::Constants params = lv::initialize_const();
  lv::Point point_i = lv::initialize_point();
  lv::Simulation prova1(point_i, params);
  lv::command prova1_run(prova1);
  prova1_run.run_command();
        }catch (const std::runtime_error& e){
             std::cerr <<e.what() << ". Uscita dal programma.\n";
       return 1;
       
            }
            return 0;
}

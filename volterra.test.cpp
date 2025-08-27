#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "volterra.hpp"

#include <sstream>
#include <string>

#include "command.hpp"
#include "doctest.h"
using std::istringstream;
lv::Constants case1{1., 1., 1., 1.};
lv::Constants case2{1.12, 6.43, 2.0, 3.15};
lv::Constants case3{0.5, 0.2, 0.4, 0.01};
lv::Constants case4{1.6, 2.0, 0.04, 1.2};
lv::Point p2{500, 45};
lv::Point p5{16, 8};
TEST_CASE("Testing evolution of the evolution vector") {
  lv::Simulation sim_a(p2, case3);
  SUBCASE("Testing ev_vector_ size after evolution") {
    CHECK(sim_a.evolution_vector().size() == 1);
    sim_a.evolve(0.001, 1492);
    CHECK(sim_a.evolution_vector().size() == 1493);
  }
  SUBCASE("Testing negative number of steps") {
    CHECK_THROWS_WITH(
        sim_a.evolve(0.001, -3),
        "Numero di step non valido o dt fuori ordine di grandezza");
  }
  SUBCASE("Testing zero number of steps") {
    CHECK_THROWS_WITH(
        sim_a.evolve(0.001, 0),
        "Numero di step non valido o dt fuori ordine di grandezza");
  }
  SUBCASE("Testing too big dt") {
    CHECK_THROWS_WITH(
        sim_a.evolve(0.3, 1),
        "Numero di step non valido o dt fuori ordine di grandezza");
  }
  SUBCASE("Testing too small dt") {
    CHECK_THROWS_WITH(
        sim_a.evolve(0.0001, 1),
        "Numero di step non valido o dt fuori ordine di grandezza");
  }
  SUBCASE("Testing zero dt") {
    CHECK_THROWS_WITH(
        sim_a.evolve(0, 1),
        "Numero di step non valido o dt fuori ordine di grandezza");
  }
  SUBCASE("Testing overflows with big initial point") {  // evolve gestisce
                                                         // anche i casi di
                                                         // overflow o Nan di H
    lv::Point p_big{1000, 1000};
    lv::Simulation sim_b(p_big, case3);
    CHECK_THROWS_WITH(sim_b.evolve(0.002, 15),
                      "Simulazione non gestibile a causa di overflow");
  }
  SUBCASE("Testing overflows with big initial constants") {
    lv::Constants big_const(1000, 1000, 1000, 1000);
    lv::Simulation sim_b(p2, big_const);
    CHECK_THROWS_WITH(sim_b.evolve(0.002, 15),
                      "Simulazione non gestibile a causa di overflow");
  }
}

TEST_CASE("Indirect testing of Simulation private methods") {
  lv::Point p_eq_case2(1.58, 0.17);
  SUBCASE("Testing calc_next_point with valid point via e_evolution") {
    lv::Simulation sim(p2, case4);
    sim.evolve(0.009, 1);
    const lv::Point& last_point = sim.evolution_vector().back();
    CHECK(last_point.x == doctest::Approx(3.4).epsilon(0.01));
    CHECK(last_point.y == doctest::Approx(65.8).epsilon(0.01));
    CHECK(last_point.H == doctest::Approx(123.5).epsilon(0.01));
  }
  SUBCASE("Testing calc_next_point with equilibrium point via e_evolution") {
    lv::Simulation sim(p_eq_case2, case2);
    sim.evolve(0.001, 1000);
    const lv::Point& last_point = sim.evolution_vector().back();
    CHECK(last_point.x == doctest::Approx(1.0).epsilon(0.01));
    CHECK(last_point.y == doctest::Approx(1.0).epsilon(0.01));
    CHECK(last_point.H == doctest::Approx(8.43).epsilon(0.01));
  }
}
TEST_CASE("Testing get_step_result") {
  lv::Simulation sim(p5, case1);
  sim.evolve(0.001, 3);
  SUBCASE("Testing valid step request") {
    lv::Point p = sim.get_step_result(2);
    CHECK(p.x == doctest::Approx(15.8).epsilon(0.01));
    CHECK(p.y == doctest::Approx(8.2).epsilon(0.01));
    CHECK(p.H == doctest::Approx(19.1).epsilon(0.01));
  }
  SUBCASE("Testing negative step request") {
    CHECK_THROWS_WITH(sim.get_step_result(-1),
                      "Non esiste un step alla posizione richiesta");
  }
  SUBCASE("Testing over size step request") {
    CHECK_THROWS_WITH(sim.get_step_result(6),
                      "Non esiste un step alla posizione richiesta");
  }
  SUBCASE("Testing returning first point") {
    lv::Point p = sim.get_step_result(0);
    CHECK(p.x == doctest::Approx(16.0).epsilon(0.01));
    CHECK(p.y == doctest::Approx(8.0).epsilon(0.01));
    CHECK(p.H == doctest::Approx(19.1).epsilon(0.01));
  }
}
TEST_CASE("Testing the conversion") {
  lv::Point p1{0.4, 3.};

  lv::Point p4{20, 220};

  SUBCASE("Testing absolute to relative") {
    lv::abs_to_rel(p1, case1);
    lv::abs_to_rel(p2, case4);
    CHECK(p1.x == 0.4);
    CHECK(p1.y == 3.);
    CHECK(p2.x == doctest::Approx(16.6).epsilon(0.01));
    CHECK(p2.y == doctest::Approx(56.3).epsilon(0.01));
  }
  SUBCASE("Testing relative to absolute") {
    lv::rel_to_abs(p4, case1);
    lv::rel_to_abs(p5, case2);
    CHECK(p4.x == 20);
    CHECK(p4.y == 220);
    CHECK(p5.x == doctest::Approx(25.2).epsilon(0.01));
    CHECK(p5.y == doctest::Approx(1.4).epsilon(0.01));
  }
}
TEST_CASE("Testing functions that read from user") {
  lv::Constants k;
  lv::Point p_zero;
  SUBCASE("Testing valid input for constants") {
    std::istringstream ok_input("1.0 1.1 3.0 8.3");
    std::cin.rdbuf(ok_input.rdbuf());
    initialize_const(k);
    CHECK(k.a == doctest::Approx(1.0));
    CHECK(k.b == doctest::Approx(1.1));
    CHECK(k.c == doctest::Approx(3.0));
    CHECK(k.d == doctest::Approx(8.3));
  }
  SUBCASE("Testing invalid numeric input for constants") {
    std::istringstream inv_input("1.0 -3.0 2.0 6.0");
    std::cin.rdbuf(inv_input.rdbuf());
    CHECK_THROWS(lv::initialize_const(k));
  }

  SUBCASE("Testing non-numeric input for constants") {
    std::istringstream non_num_input("ciao");
    std::cin.rdbuf(non_num_input.rdbuf());
    CHECK_THROWS(lv::initialize_const(k));
  }
  SUBCASE("Testing void input for constants") {
    std::istringstream no_input("");
    std::cin.rdbuf(no_input.rdbuf());
    CHECK_THROWS(lv::initialize_const(k));
  }

  SUBCASE("Testing void input for point zero") {
    std::istringstream no_p0("");
    std::cin.rdbuf(no_p0.rdbuf());
    CHECK_THROWS(lv::set_zeropoint(p_zero));
  }
  SUBCASE("Testing invalid input for point zero") {
    std::istringstream inv_p0("-3 1");
    std::cin.rdbuf(inv_p0.rdbuf());
    CHECK_THROWS(lv::set_zeropoint(p_zero));
  }
  SUBCASE("Testing non-numeric input for point zero") {
    std::istringstream inv_p0("pippi");
    std::cin.rdbuf(inv_p0.rdbuf());
    CHECK_THROWS(lv::set_zeropoint(p_zero));
  }
}
TEST_CASE("Indirect testing of command private methods") {
  lv::Simulation sim({1.0, 1.0, 0.0}, case1);
  lv::command cmd(sim);
  SUBCASE("Testing invalid input nsteps for set_time via run_command()") {
    std::istringstream inv_nsteps("0\n0.001\ne\nq\n");
    std::cin.rdbuf(inv_nsteps.rdbuf());
    CHECK_THROWS(cmd.run_command());
  }
  SUBCASE("Testing invalid input dt (too big) for set_time via run_command()") {
    std::istringstream inv_nsteps("1\n10\ne\nq\n");
    std::cin.rdbuf(inv_nsteps.rdbuf());
    CHECK_THROWS(cmd.run_command());
  }
  SUBCASE(
      "Testing invalid input dt (too small) for set_time via run_command()") {
    std::istringstream inv_nsteps("1\n0.0001\ne\nq\n");
    std::cin.rdbuf(inv_nsteps.rdbuf());
    CHECK_THROWS(cmd.run_command());
  }
  SUBCASE("Testing non-numeric input nstep for set_time via run_command()") {
    std::istringstream inv_nsteps("ciao\n0.001\ne\nq\n");
    std::cin.rdbuf(inv_nsteps.rdbuf());
    CHECK_THROWS(cmd.run_command());
  }
  SUBCASE("Testing non-numeric input nstep for set_time via run_command()") {
    std::istringstream inv_nsteps("1\nmalva\ne\nq\n");
    std::cin.rdbuf(inv_nsteps.rdbuf());
    CHECK_THROWS(cmd.run_command());
  }
}

TEST_CASE("Testing user inputs for run_command method") {
  lv::Simulation sim({1.0, 1.0, 0.0}, case1);
  lv::command cmd(sim);

  SUBCASE("Testing not existing command") {
    std::istringstream inv_command("s");
    std::cin.rdbuf(inv_command.rdbuf());
    CHECK_THROWS_WITH(cmd.run_command(), "Comando inesistente");
  }
  SUBCASE("Testing numeric input") {
    std::istringstream numeric("5");
    std::cin.rdbuf(numeric.rdbuf());
    CHECK_THROWS_WITH(cmd.run_command(), "Comando inesistente");
  }
  SUBCASE("Testing more than one character input") {
    std::istringstream word("caffe");
    std::cin.rdbuf(word.rdbuf());
    CHECK_THROWS_WITH(cmd.run_command(), "Inserire un solo carattere");
  }
  SUBCASE("Testing enter input") {
    std::istringstream nothing("\n");
    std::cin.rdbuf(nothing.rdbuf());
    CHECK_THROWS_WITH(cmd.run_command(), "Inserire un solo carattere");
  }
  SUBCASE(
      "Testing "
      " input") {
    std::istringstream nothing(" ");
    std::cin.rdbuf(nothing.rdbuf());
    CHECK_THROWS_WITH(cmd.run_command(), "Comando inesistente");
  }
  SUBCASE("Testing 'c' before 'e'") {
    std::istringstream c_e("c\nq\n");
    std::cin.rdbuf(c_e.rdbuf());
    CHECK_THROWS_WITH(cmd.run_command(),
                      "Impossibile continuare simulazione non esistente");
  }
}

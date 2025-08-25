#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "volterra.hpp"

#include "doctest.h"
TEST_CASE("Testing volterra simulation") {
  SUBCASE("invalid points") {
    CHECK_THROWS(lv::Simulation(
        {-2., 4.},
        {1., 2., 3.,
         4.}));  // senza nome è un espressione, con nome dichiarazione
    CHECK_THROWS(lv::Simulation({2., -4.}, {1., 2., -3., 4.}));
  }
  SUBCASE("invalid constants") {
    CHECK_THROWS(lv::Simulation({-2., 4.}, {1., -2., 3., 0.}));
    CHECK_THROWS(lv::Simulation({2., 4.}, {1., -2., -3., -4.}));
  }
}
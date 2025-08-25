#include "volterra.hpp"

#include <vector>
namespace lv {
Constants initialize_const() {
  double a, b, c, d;
  do {
    std::cin >> a >> b >> c >> d;
    if (a < 0 || b < 0 || c < 0 || d < 0) {
      throw std::runtime_error{"Parametri errati"};
    }
  } while (a < 0 || b < 0 || c < 0 || d < 0);
  return Constants{a, b, c, d};
}
Point initialize_point() {
  double x0, y0;
  do {
    std::cin >> x0 >> y0;
    if (x0 < 0 || y0) {
      throw std::runtime_error{"Parametri errati"};
    }
  } while (x0 < 0 || y0 < 0);

  return Point{x0, y0};
}
Point Simulation::calc_point(double x_prev, double y_prev, double dt) {
  double x = x_prev + parameters.a * (1 - y_prev) * x_prev * dt;
  double y = y_prev + parameters.d * (x_prev - 1) * y_prev * dt;
  return Point(x, y);
}
Simulation::Simulation(const Point& p_i, const Constants& c) : parameters(c) {
  ev_vector_.push_back(give_relative_point(p_i));
};
void Simulation::e_evolve(double dt, int nsteps) {
  for (int i = 0; i < nsteps; i++) {
    const Point& last = ev_vector_.back();
    Point next{calc_point(last.x, last.y, dt)};
    ev_vector_.push_back(next);
  }
}
}  // namespace lv

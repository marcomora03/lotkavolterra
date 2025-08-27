#include "volterra.hpp"

#include <cmath>
#include <stdexcept>
#include <vector>
namespace lv {

void initialize_const(Constants& k) {
  double a, b, c, d;
  std::cout << "inserire parametri: a(fertlità prede) b(mortalità prede) "
               "c(fertlità predatori) d(mortalità predatori)"
            << std::endl;
  std::cin >> a >> b >> c >> d;
  if (std::cin.fail() || a < 0 || b < 0 || c < 0 || d < 0) {
    throw std::runtime_error("Parametri non validi");
  }
  k.a = a;
  k.b = b;
  k.c = c;
  k.d = d;
}
void set_zeropoint(Point& p0) {
  double x0, y0;
  std::cout << "inserire valori inziali di densità di popolazione delle prede "
               "(x) e dei predatori (y)"
            << std::endl;
  std::cin >> x0 >> y0;
  if (std::cin.fail() || x0 <= 0 || y0 <= 0) {
    throw std::runtime_error("Punti non validi");
  }
  p0.x = x0;
  p0.y = y0;
}
double Simulation::calculate_H(double x, double y) const {
  double H = -parameters.d * std::log(x) + parameters.c * x + parameters.b * y -
             parameters.a * std::log(y);
  return H;
}
void abs_to_rel(Point& p, const Constants& c) {
  p.x = p.x * (c.c / c.d);
  p.y = p.y * (c.b / c.a);
}
void rel_to_abs(Point& p, const Constants& c) {
  p.x = p.x * (c.d / c.c);
  p.y = p.y * (c.a / c.b);
}
Point Simulation::calc_next_point(const Point& prev_point, double dt) const {
  double x =
      prev_point.x + parameters.a * (1 - prev_point.y) * prev_point.x * dt;
  double y =
      prev_point.y + parameters.d * (prev_point.x - 1) * prev_point.y * dt;
  double H = calculate_H(x, y);
  return Point{x, y, H};
}
Simulation::Simulation(const Point& p_i, const Constants& c) : parameters(c) {
  Point p_rel = p_i;
  abs_to_rel(p_rel, c);
  p_rel.H = calculate_H(p_rel.x, p_rel.y);
  ev_vector_.push_back(p_rel);
};
void Simulation::evolve(double dt, int nsteps) {
  if (nsteps <= 0 || dt < 0.001 || dt > 0.009) {
    throw std::runtime_error(
        "Numero di step non valido o dt fuori ordine di grandezza");
  }
  for (int i = 0; i < nsteps; i++) {
    const Point& last = ev_vector_.back();
    Point next{calc_next_point(last, dt)};
    if (!std::isfinite(next.x) || !std::isfinite(next.y) ||
        !std::isfinite(next.H)) {
      throw std::runtime_error("Simulazione non gestibile a causa di overflow");
    }
    ev_vector_.push_back(next);
  }
}

Point Simulation::get_step_result(int n) const {
  if (n < 0 || static_cast<size_t>(n) >= ev_vector_.size()) {
    throw std::runtime_error("Non esiste un step alla posizione richiesta");
  }
  Point p_abs = ev_vector_[n];
  rel_to_abs(p_abs, parameters);
  p_abs.H = calculate_H(p_abs.x, p_abs.y);
  return p_abs;
}
}  

#ifndef LV_REGRESSION_HPP  // è una macro puo avere qualsiasi nome ma si usa la
                           // convenzione qua usata (nome file maiusc)
#define LV_REGRESSION_HPP
#include <iostream>
#include <stdexcept>
#include <vector>
namespace lv {
struct Point {
  double x;
  double y;
  Point() = default;                             // costruttore di default
  Point(double x_, double y_) : x(x_), y(y_) {}  // costruttore parametrizzato
};
struct Constants {
  const double a, b, c, d;
  Constants(double a_, double b_, double c_, double d_)
      : a(a_), b(b_), c(c_), d(d_) {}
};
Constants initialize_const();
Point initialize_point();

class Simulation {
 private:
  Constants parameters;
  std::vector<Point> ev_vector_;
  Point give_relative_point(const Point& p) {
    double x_rel = p.x * (parameters.c / parameters.d);
    double y_rel = p.y * (parameters.b / parameters.a);
    return Point{x_rel, y_rel};
  }
  Point calc_point(double x_prev, double y_prev, double dt);

 public:
  Simulation(const Point& p_i, const Constants& c);

  // costruttore che inserisce in evvector il p iniziale inseriti da utente
  // inizializza i parametri
  const std::vector<Point>& evolution_vector() const { return ev_vector_; }
  void e_evolve(double dt, int nsteps);

  const Point& a_p_get_step_result(int n) {
    static Point
        res;  // static fa persistere un oggetto oltre il suo normale scope
    res.x = ev_vector_[n].x * (parameters.d / parameters.c);
    res.y = ev_vector_[n].y * (parameters.a / parameters.b);
    return res;
  }
  void initialize(Simulation& sim) {}
};
}  // namespace lv
#endif
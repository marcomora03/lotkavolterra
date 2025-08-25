#include "volterra.hpp"
#include <cmath>
#include <vector>
namespace lv {
  size_t Simulation::size() const
{
  return ev_vector_.size();
}
Constants initialize_const() {
  double a, b, c, d;
  do {
    std::cin >> a >> b >> c >> d;
    if (a < 0 || b < 0 || c < 0 || d < 0) {
      std::cout<<"Parametri errati"<<std::endl;}
    }
  while (a < 0 || b < 0 || c < 0 || d < 0);
  return Constants{a, b, c, d};
}
Point initialize_point() {
  double x0, y0;
  do {
    std::cin >> x0 >> y0;
    if (x0 < 0 || y0<0) {
      std::cout<<"Inserire punti maggiori o uguali a zero"<<std::endl;}
    }
   while (x0 < 0 || y0 < 0);

  return Point{x0, y0};
}
Point Simulation::calc_point(double x_prev, double y_prev,double H, double dt) {
  double x = x_prev + parameters.a * (1 - y_prev) * x_prev * dt;
  double y = y_prev + parameters.d * (x_prev - 1) * y_prev * dt;
  H = -parameters.d*std::log(x)+parameters.c*x+parameters.b*y-parameters.a*std::log(y);
  return Point(x, y, H);
}
Simulation::Simulation(const Point& p_i, const Constants& c) : parameters(c) {
  ev_vector_.push_back(give_relative_point(p_i));
};
void Simulation::e_evolve(double dt, int nsteps) {
  for (int i = 0; i < nsteps; i++) {
    const Point& last = ev_vector_.back();
    Point next{calc_point(last.x, last.y,last.H, dt)};
    ev_vector_.push_back(next);
  }
}
  Point Simulation::give_relative_point(const Point& p) {
    double x_rel = p.x * (parameters.c / parameters.d);
    double y_rel = p.y * (parameters.b / parameters.a);
    return Point{x_rel, y_rel};
  }
   const Point& Simulation::a_p_get_step_result(int n) {
    static Point
        res;  // static fa persistere un oggetto oltre il suo normale scope
    res.x = ev_vector_[n].x * (parameters.d / parameters.c);
    res.y = ev_vector_[n].y * (parameters.a / parameters.b);
      res.H =-parameters.d*std::log(ev_vector_[n].x)+parameters.c*ev_vector_[n].x+parameters.b*ev_vector_[n].y-parameters.a*std::log(ev_vector_[n].y);
    return res;
  }
}  // namespace lv

#include "volterra.hpp"
#include <cmath>
#include <vector>
namespace lv {
Constants initialize_const() {
  double a, b, c, d;
  std::cout<<"inserire parametri: a(fertlità prede) b(mortalità prede) c(fertlità predatori) d(mortalità predatori)"<<std::endl;
    std::cin >> a >> b >> c >> d;
    if (a < 0 || b < 0 || c < 0 || d < 0) {
     
     throw std::runtime_error("Parametri non validi");}
  return Constants{a, b, c, d};
}
Point initialize_point() {
  double x0, y0;
    std::cout<<"inserire valori inziali di densità di popolazione delle prede (x) e dei predatori (y)"<<std::endl;
    std::cin >> x0 >> y0;
    if (x0 < 0 || y0<0) {
      throw std::runtime_error("Punti non validi");}
    

  return Point{x0, y0};
}
double Simulation::calculate_H(double x, double y){
  return -parameters.d*std::log(x)+parameters.c*x+parameters.b*y-parameters.a*std::log(y);
}
Point Simulation::calc_next_point(const Point& prev_point,  double dt) {
  double x = prev_point.x + parameters.a * (1 - prev_point.y) *  prev_point.x * dt;
  double y = prev_point.y + parameters.d * ( prev_point.x - 1) * prev_point.y * dt;
  double H = calculate_H(x* (parameters.d / parameters.c),y* (parameters.a / parameters.b));
  return Point{x, y, H};
}
Simulation::Simulation(const Point& p_i, const Constants& c) : parameters(c) {
  
  ev_vector_.push_back(give_relative_point(p_i));
};
void Simulation::e_evolve(double dt, int nsteps) {
  for (int i = 0; i < nsteps; i++) {
    const Point& last = ev_vector_.back();
    Point next{calc_next_point(last, dt)};
    ev_vector_.push_back(next);
  }
}
  Point Simulation::give_relative_point(const Point& p) {
    double x_rel = p.x * (parameters.c / parameters.d);
    double y_rel = p.y * (parameters.b / parameters.a);
    return Point{x_rel, y_rel};
  }
    Point Simulation::a_p_get_step_result(int n) {  //a_p_ a inizio nome è un riferimento ai comandi che faranno uso di questo metodo, cosi come e_ in e_evolve
    if (n < 0 || static_cast<size_t>(n) >= ev_vector_.size()) {
throw std::runtime_error("non esiste un step alla posizione richiesta");}
    double x_abs = ev_vector_[n].x * (parameters.d / parameters.c);
    double y_abs = ev_vector_[n].y * (parameters.a / parameters.b);
    double H = calculate_H(x_abs,y_abs);
    return Point{x_abs,y_abs,H}; 
  
}  
}

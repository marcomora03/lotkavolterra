#ifndef LV_REGRESSION_HPP
#define LV_REGRESSION_HPP
#include <iostream>
#include <vector>
namespace lv {
struct Point {
  double x;
  double y;
  double H;
  Point() : x(0.0), y(0.0), H(0.0) {}
  Point(double x_, double y_, double H_ = 0.0) : x(x_), y(y_), H(H_) {}
};
struct Constants {
  double a, b, c, d;
  Constants() : a(0), b(0), c(0), d(0) {}
  Constants(double a_, double b_, double c_, double d_)
      : a(a_), b(b_), c(c_), d(d_) {}
};
void initialize_const(Constants& k);
void set_zeropoint(Point& p0);
void abs_to_rel(Point& p, const Constants& c);
void rel_to_abs(Point& p, const Constants& c);
class Simulation {
 private:
  Constants parameters;
  std::vector<Point> ev_vector_;
  Point calc_next_point(const Point& prev_point, double dt) const;
  double calculate_H(double x, double y) const;

 public:
  Simulation(const Point& p_i, const Constants& c);

  const std::vector<Point>& evolution_vector() const { return ev_vector_; }
  void evolve(double dt, int nsteps);

  Point get_step_result(int n) const;
};
}  
#endif

#ifndef MYBSPLINECURVE_H
#define MYBSPLINECURVE_H

#include <gmParametricsModule>

using control_points = GMlib::DVector<GMlib::Vector<float, 3>>;

namespace MySoothingNamespace {
class MyVisualizer;

class MyBSplineCurve : public GMlib::PBSplineCurve<float> {
  public:
  MyBSplineCurve(const control_points& c, const GMlib::DVector<float>& t, int d = 0); // maybe unused, kept for safety.
  MyBSplineCurve(const control_points& c, int d, bool closed);
  MyBSplineCurve(const MyBSplineCurve& curve);

  using PBSplineCurve::PBSplineCurve;

  protected:
  void localSimulate(double dt) override;

  private:
  void createVisualizer();
  bool checkStateChange();

  float _previousLength;
  std::unique_ptr<MyVisualizer> _visualizer;
};

} // namespace MySoothingNamespace
#endif // MYBSPLINECURVE_H

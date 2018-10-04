#ifndef MYBSPLINECURVE_H
#define MYBSPLINECURVE_H

#include <gmParametricsModule>

namespace MySoothingNamespace {
class MyVisualizer;

class mybsplinecurve : public GMlib::PBSplineCurve<float> {
  public:
  mybsplinecurve(const GMlib::DVector<GMlib::Vector<float, 3>>& c, const GMlib::DVector<float>& t, int d = 0);
  mybsplinecurve(const GMlib::DVector<GMlib::Vector<float, 3>>& c, int d, bool closed);
  //mybsplinecurve(const GMlib::PCurve<float,3>& curve, int d, int n, int m);
  mybsplinecurve(const mybsplinecurve& curve);

  bool _stateChanged;

  using PBSplineCurve::PBSplineCurve;

  protected:
  void localSimulate(double dt) override;

  private:
  void createVisualizer();
  void checkStateChange();

  float _previousLength;
  std::unique_ptr<MyVisualizer> _visualizer;
};

} // namespace MySoothingNamespace
#endif // MYBSPLINECURVE_H

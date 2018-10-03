#ifndef MYBSPLINECURVE_H
#define MYBSPLINECURVE_H

#include <gmParametricsModule>

namespace MySoothingNamespace {

class mybsplinecurve : public GMlib::PBSplineCurve<float> {
  public:
  using PBSplineCurve::PBSplineCurve;

  protected:
  void localSimulate(double dt) override;
};

} // namespace MySoothingNamespace
#endif // MYBSPLINECURVE_H

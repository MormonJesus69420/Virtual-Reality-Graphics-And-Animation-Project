#include "mybsplinecurve.h"

//Our headers
#include "myvisualizer.h"

namespace MySoothingNamespace {

mybsplinecurve::mybsplinecurve(const GMlib::DVector<GMlib::Vector<float, 3>>& c, const GMlib::DVector<float>& t, int d)
    : GMlib::PBSplineCurve<float>(c, t, d)
{
  createVisualizer();
}

mybsplinecurve::mybsplinecurve(const mybsplinecurve& curve)
    : GMlib::PBSplineCurve<float>(curve)
    , _visualizer(new MyVisualizer(*(curve._visualizer), this))
{
}

mybsplinecurve::mybsplinecurve(const GMlib::DVector<GMlib::Vector<float, 3>>& c, int d, bool closed)
    : GMlib::PBSplineCurve<float>(c, d, closed)
{
  createVisualizer();
}

void mybsplinecurve::localSimulate(double dt [[maybe_unused]])
{
  if (checkStateChange())
    _visualizer->updateParams();
}

bool mybsplinecurve::checkStateChange()
{
  float length = getCurveLength();

  if (fabs(_previousLength - length) > 0.0001f) {
    _previousLength = length;
    return true;
  }

  return false;
}

void mybsplinecurve::createVisualizer()
{
  _previousLength = getCurveLength();
  _visualizer = std::make_unique<MyVisualizer>(this);
  _visualizer->visualize();
}

} // namespace MySoothingNamespace

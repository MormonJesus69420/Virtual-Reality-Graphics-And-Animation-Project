#include "mybsplinecurve.h"

//Our headers
#include "myvisualizer.h"

namespace MySoothingNamespace {

MyBSplineCurve::MyBSplineCurve(const GMlib::DVector<GMlib::Vector<float, 3>>& c, const GMlib::DVector<float>& t, int d)
    : GMlib::PBSplineCurve<float>(c, t, d)
{
  createVisualizer();
}

MyBSplineCurve::MyBSplineCurve(const MyBSplineCurve& curve)
    : GMlib::PBSplineCurve<float>(curve)
    , _visualizer(new MyVisualizer(*(curve._visualizer), this))
{
}

MyBSplineCurve::MyBSplineCurve(const GMlib::DVector<GMlib::Vector<float, 3>>& c, int d, bool closed)
    : GMlib::PBSplineCurve<float>(c, d, closed)
{
  createVisualizer();
}

void MyBSplineCurve::localSimulate(double dt [[maybe_unused]])
{
  if (checkStateChange())
    _visualizer->updateParams();
}

bool MyBSplineCurve::checkStateChange()
{
  float length = getCurveLength();

  if (fabs(_previousLength - length) > 0.0001f) {
    _previousLength = length;
    return true;
  }

  return false;
}

void MyBSplineCurve::createVisualizer()
{
  _previousLength = getCurveLength();
  _visualizer = std::make_unique<MyVisualizer>(this);
  _visualizer->visualize();
}

MyVisualizer* MyBSplineCurve::getVisualizer(){
  return _visualizer.get();
}

} // namespace MySoothingNamespace

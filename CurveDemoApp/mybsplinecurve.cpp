#include "mybsplinecurve.h"

//Our headers
#include "myvisualizer.h"

namespace MySoothingNamespace {


mybsplinecurve::mybsplinecurve(const GMlib::DVector< GMlib::Vector<float, 3> >& c, const GMlib::DVector<float>& t, int d) : GMlib::PBSplineCurve<float>(c,t,d)
{
  createVisualizer();
}

mybsplinecurve::mybsplinecurve(const mybsplinecurve &curve) : GMlib::PBSplineCurve<float>(curve), _visualizer(new MyVisualizer(*(curve._visualizer), this))
{
}

//mybsplinecurve::mybsplinecurve(const GMlib::PCurve<float, 3> &curve, int d, int n, int m) : GMlib::PBSplineCurve<float>(curve, d, n, m)
//{
//  createVisualizer();
//}

mybsplinecurve::mybsplinecurve(const GMlib::DVector<GMlib::Vector<float, 3> > &c, int d, bool closed) : GMlib::PBSplineCurve<float>(c, d, closed)
{
  createVisualizer();
}

void mybsplinecurve::localSimulate(double dt)
{
  _visualizer->localSimulate(dt);
  //rotate(GMlib::Angle(30) * dt, GMlib::Vector<float, 3>(1.0f, 0.0f, 0.0f));
}

void mybsplinecurve::createVisualizer()
{
  _visualizer = std::make_unique<MyVisualizer>(this);
  _visualizer->visualize();
}

} // namespace MySoothingNamespace

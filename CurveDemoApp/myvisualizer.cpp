#include "myvisualizer.h"
#include "gmParametricsModule"
#include "myviscircle.h"

//stl
#include <math.h>

using GMVec3 = GMlib::Vector<float, 3>;

namespace MySoothingNamespace {

MyVisualizer::MyVisualizer(const GMlib::PBSplineCurve<float>* c, const float tStep)
{
  _wrapper = &(GMlibWrapper::instance());
  if (_wrapper == nullptr) { // If this happens, we tried to get the wrapper too early
    throw std::runtime_error("Wrapper should not be nullptr, check if the visualizer was constructed too early");
  }

  this->_curve = c;
  this->_tStep = tStep;
}

MyVisualizer::MyVisualizer(const MyVisualizer& vis, const GMlib::PBSplineCurve<float>* c)
{
  this->_curve = c;
  this->_tStep = vis._tStep;
  this->_params = vis._params;
  this->_wrapper = vis._wrapper;
}

bool MyVisualizer::visualize()
{
  try {
    setupParams();
    setupCircles();

    return true;
  }
  catch (std::exception e) {
    std::cerr << e.what() << std::endl;

    return false;
  }
}

void MyVisualizer::setupParams()
{
  auto lim = _curve->getParEnd();

  for (auto t = 0.0f; t <= lim; t += _tStep)
    setupParam(t);
}

void MyVisualizer::setupParam(const float t)
{
  float curvature = _curve->getCurvature(t);
  GMVec3 position = _curve->getPosition(t);
  float torsion = calculateTorsion(t);
  GMVec3 tangent = _curve->getDer1(t);

  _params.emplace(_params.end(), t, curvature, torsion, position, tangent);
}

float MyVisualizer::calculateTorsion(const float t) const
{
  auto cross = _curve->getDer1(t) ^ _curve->getDer2(t);

  if (cross.getLength() <= 0)
    throw std::invalid_argument("Error, cross product is zero");

  return cross * _curve->getDer3(t) / powf(cross.getLength(), 2);
}

void MyVisualizer::setupCircles()
{
  findGreatestTorsion(); // Done here so we only calculate it once, used for calculateColor

  for (auto& param : _params) {
    auto* mycircle = new MyVisualizerCircle<float>(param.curvature * 0.05f);

    param.circle = mycircle;
    moveCircleToCurve(param);

    mycircle->setColor(calculateColor(param));
    mycircle->toggleDefaultVisualizer();
    mycircle->sample(100, 2);

    _wrapper->scene()->insert(mycircle);
  }
}

void MyVisualizer::moveCircleToCurve(CurveParams& p)
{
  p.tangent[0] *= -1;
  p.circle->set(p.position, p.tangent, _up_vector);
}

GMlib::Color MyVisualizer::calculateColor(const MyVisualizer::CurveParams& p)
{
  // Set HUE val, 0.66 (blue) for 0 torsion, 0.00 (red) for negative, 0.33 (green) otherwise.
  double hue = fabs(p.torsion) < 0.1f ? 0.66 : p.torsion < 0.0f ? 0.0 : 0.33;
  // Set SATURATION, relativistic to the greatest torsion.
  double sat = double(abs(p.torsion) / _greatest_torsion + 0.1f);

  auto color = GMlib::Color(hue, sat > 1.0 ? 1.0 : sat, 1.0, 1.0);
  color.toRGB();
  return color;
}

void MyVisualizer::findGreatestTorsion()
{
  auto maxIt = std::max_element(_params.begin(), _params.end(),
      [](const CurveParams& obj1, const CurveParams& obj2) {
        return fabs(obj1.torsion) < fabs(obj2.torsion);
      });
  if (maxIt == _params.end()) //for some reason (this should never happen as long as there are elements)
    return;

  _greatest_torsion = fabs(maxIt->torsion);
}

void MyVisualizer::updateParams()
{
  for (auto& param : _params)
    updateParam(param);

  findGreatestTorsion();

  for (auto& p : _params) {
    p.circle->setColor(calculateColor(p));
  }
}

void MyVisualizer::updateParam(MyVisualizer::CurveParams& p)
{
  float t = p.t;

  p.curvature = _curve->getCurvature(t);
  p.position = _curve->getPosition(t);
  p.torsion = calculateTorsion(t);
  p.tangent = _curve->getDer1(t);

  p.circle->setRadius(p.curvature * 0.05f);
  moveCircleToCurve(p);

  p.circle->sample(100, 2);
}

MyVisualizer::CurveParams::CurveParams(MyVisualizer::CurveParams &&p)
  : t(std::move(p.t)),
    curvature(std::move(p.curvature)),
    torsion(std::move(p.torsion)),
    position(p.position),
    tangent(p.tangent),
    circle(std::move(p.circle))
{
}

MyVisualizer::CurveParams::CurveParams(float t, float curvature, float torsion, const GMVec3& position, const GMVec3& tangent)
{
  this->t = t;
  this->curvature = curvature;
  this->torsion = torsion;
  this->position = position;
  this->tangent = tangent;
}

MyVisualizer::CurveParams& MyVisualizer::CurveParams::operator=(MyVisualizer::CurveParams &&p)
{
  t = std::move(p.t);
  curvature = std::move(p.curvature);
  torsion = std::move(p.torsion);
  position = p.position;
  tangent = p.tangent;
  circle = std::move(p.circle);

  return *this;
}

} // namespace MySoothingNamespace

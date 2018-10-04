#include "myvisualizer.h"
#include "gmParametricsModule"
#include "myviscircle.h"

//stl
#include <math.h>

using GMVec3 = GMlib::Vector<float, 3>;

namespace MySoothingNamespace {

MyVisualizer::MyVisualizer(const GMlib::PBSplineCurve<float>* c, int sampleSize, const float tStep)
{
  _wrapper = &(GMlibWrapper::instance());
  if (_wrapper == nullptr) { // If this happens, we tried to get the wrapper too early
    throw std::runtime_error("Wrapper should not be nullptr, check if the visualizer was constructed too early");
  }

  this->_circles = std::vector<MyVisualizerCircle<float>*>();
  this->_curve = c;
  this->_samples = sampleSize;
  this->_tStep = tStep;
}

MyVisualizer::MyVisualizer(const MyVisualizer &vis, const GMlib::PBSplineCurve<float> *c)
{
  this->_circles = vis._circles;
  this->_curve = c;
  this->_samples = vis._samples;
  this->_tStep = vis._tStep;
  this->_params = vis._params;
  this->_wrapper = vis._wrapper;
}

bool MyVisualizer::visualize()
{
  try {
    setupParams();
    for (auto& param : _params) {
      auto* mycircle = new MyVisualizerCircle<float>(param.t, param.curvature * 0.05f);

      param.circle = mycircle;
      moveCircleToCurve(param);

      mycircle->toggleDefaultVisualizer();
      auto color = this->calculateColor(param);
      mycircle->setColor(color);
      mycircle->sample(100, 2);
      this->_circles.push_back(mycircle);
      this->_wrapper->scene()->insert(mycircle);
    }
    return true;
  }
  catch (std::exception e) {
    std::cerr << e.what();
    return false;
  }
}

float MyVisualizer::findGreatestTorsion() const
{
  auto maxIt = std::max_element(_params.begin(), _params.end(),
      [](const CurveParams& obj1, const CurveParams& obj2) {
        return fabs(obj1.torsion) < fabs(obj2.torsion);
      });
  if (maxIt == _params.end()) //for some reason (this should never happen as long as there are elements)
    return 0;
  return fabs(maxIt->torsion);
}
GMlib::Color MyVisualizer::calculateColor(const MyVisualizer::CurveParams& p)
{
  float maxTorsion = this->findGreatestTorsion();
  float hue = fabs(p.torsion) < 0.1f ? 0.66f : p.torsion < 0.0f ? 0.0f: 0.33f;
  float sat = (fabs(p.torsion)/maxTorsion) + 0.1f;
  if (sat > 1.0f)
    sat = 1.0f;
  float val = 1.0f;

  auto color = GMlib::Color(double(hue), double(sat), double(val), 1.0);
  color.toRGB();
  return color;
}

void MyVisualizer::localSimulate(double dt [[maybe_unused]])
{
    updateParams();
}

void MyVisualizer::updateParams()
{
  for (auto& param : _params)
    updateParam(param);
}

void MyVisualizer::updateParam(MyVisualizer::CurveParams& p)
{
  float t = p.t;
  p.curvature = _curve->getCurvature(t);
  p.torsion = calculateTorsion(t);
  p.pos = _curve->getPosition(t);
  p.der1 = _curve->getDer1(t);


  p.circle->setColor(calculateColor(p));
  p.circle->setRadius(p.curvature * 0.05f);
  moveCircleToCurve(p);
  p.circle->sample(100, 2);
}


//Copy-assignment operator
MyVisualizer::CurveParams MyVisualizer::CurveParams::operator=(const MyVisualizer::CurveParams& p)
{
  return CurveParams(p);
}

////Copy-constructor
MyVisualizer::CurveParams::CurveParams(const CurveParams& p)
{
  this->t = p.t;
  this->curvature = p.curvature;
  this->torsion = p.torsion;
  this->der1 = p.der1;
  this->pos = p.pos;
  this->circle = p.circle;
}

//Move-constructor
MyVisualizer::CurveParams::CurveParams(MyVisualizer::CurveParams&& p)
    : t(p.t)
    , curvature(p.curvature)
    , torsion(p.torsion)
    , pos(p.pos)
    , der1(p.der1)
    , circle(p.circle)
{
}

MyVisualizer::CurveParams::CurveParams(float t, float c, float to, const GMVec3& pos, const GMVec3& d1)
{
  this->t = t;
  this->curvature = c;
  this->torsion = to;
  this->pos = pos;
  this->der1 = d1;
}

void MyVisualizer::setupParams()
{
  for (float t = 0; t <= this->_curve->getParEnd(); t += _tStep)
    setupParam(t);
}

void MyVisualizer::setupParam(const float t)
{
  float kurwa = _curve->getCurvature(t);
  float torsion = calculateTorsion(t);
  GMVec3 der1 = _curve->getDer1(t);
  GMVec3 pos = _curve->getPosition(t);

  _params.emplace(_params.end(), t, kurwa, torsion, pos, der1);
}

void MyVisualizer::moveCircleToCurve(CurveParams& p)
{
  MyVisualizerCircle<float>* circle = p.circle;

  if (circle == nullptr)
    throw std::runtime_error("Params object has no circle");

  GMVec3 coord = p.pos;
  GMVec3 der1 = p.der1;

  circle->set({ coord[0], coord[1], coord[2] },
      { -der1[0], der1[1], der1[2] },
      { 1, 0, 0 });
}

float MyVisualizer::calculateTorsion(const float t) const
{
  GMVec3 der1 = this->_curve->getDer1(t);
  GMVec3 der2 = this->_curve->getDer2(t);
  GMVec3 der3 = this->_curve->getDer3(t);

  auto cross = der1 ^ der2;
  auto dividend = cross * der3;
  auto divisor = powf(cross.getLength(), 2);

  return dividend / divisor;
}

} // namespace MySoothingNamespace

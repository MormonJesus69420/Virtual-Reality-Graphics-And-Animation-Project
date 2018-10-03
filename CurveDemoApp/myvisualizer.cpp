#include "myvisualizer.h"
#include "gmParametricsModule"
#include "myviscircle.h"

//stl
#include "cmath"

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

bool MyVisualizer::visualize()
{
  try {
    setupParams();
    for (auto& param : _params) {
      auto* mycircle = new MyVisualizerCircle<float>(param.t, param.curvature * 0.05f);

      param.circle = mycircle;
      moveCircleToCurve(param);

      mycircle->toggleDefaultVisualizer();
//      mycircle->setMaterial(GMlib::GMcolor::crimson());
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

void MyVisualizer::update(double dt)
{
  this->localSimulate(dt);
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

void MyVisualizer::updateParams()
{
  for (auto& param : _params)
    this->updateParam(param);
}

GMlib::Color MyVisualizer::calculateColor(const MyVisualizer::CurveParams& p)
{
  float maxTorsion = this->findGreatestTorsion();
  float hue = fabs(p.torsion) / maxTorsion;
  float sat = 1.0f;
  float val = 1.0f;

  auto color = GMlib::Color(double(hue), double(sat), double(val), 1.0);
  color.toRGB();
  return color;
}

void MyVisualizer::updateParam(MyVisualizer::CurveParams& p)
{
  float t = p.t;
  p.curvature = this->_curve->getCurvature(t);
  p.torsion = this->calculateTorsion(t);
  p.der1 = this->_curve->getDer1(t);

  p.circle->setRadius(p.curvature);
  this->moveCircleToCurve(p);
}

void MyVisualizer::localSimulate(double dt[[maybe_unused]])
{
  for (auto& param : this->_params)
    moveCircleToCurve(param);
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
  float kurwa = this->_curve->getCurvature(t);
  float torsion = this->calculateTorsion(t);
  GMVec3 der1 = this->_curve->getDer1(t);
  GMVec3 pos = this->_curve->getPosition(t);

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
  GMVec3 der3 = this->_curve->getDer2(t);

  float divisor = (der1 ^ der2) * der3;
  double dividend = std::pow((der1 ^ der2).getLength(), 2);

  return divisor / float(dividend);
}

float MyVisualizer::findGreatestCurvature() const
{
  auto maxIt = std::max_element(_params.begin(), _params.end(),
      [](const CurveParams& obj1, const CurveParams& obj2) {
        return obj1.curvature < obj2.curvature;
      });
  if (maxIt == _params.end()) //for some reason (this should never happen as long as there are elements)
    return 0;
  return maxIt->curvature;
}

} // namespace MySoothingNamespace

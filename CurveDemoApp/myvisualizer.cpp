#include "myvisualizer.h"
#include "gmParametricsModule"
#include "myviscircle.h"

//stl
#include "cmath"

using GMVec3 = GMlib::Vector<float, 3>;

namespace MySoothingNamespace {
MyVisualizer::MyVisualizer(GMlib::PBSplineCurve<float>* c, int sampleSize)
{
  _wrapper = &(GMlibWrapper::instance());
  if (_wrapper == nullptr) { // If this happens, we tried to get the wrapper too early
    throw std::runtime_error("Wrapper should not be nullptr, check if the visualizer was constructed too early");
  }

  this->_circles = std::vector<MyVisualizerCircle<float>*>();
  this->_curve = c;
  this->_samples = sampleSize;
}

bool MyVisualizer::visualize()
{
  try {
    for (float t = 0; t <= this->_curve->getParEnd(); t += 0.05) {

      float kurwa = this->calculateCurvature(t);
      auto* mycircle = new MyVisualizerCircle<float>(t, kurwa);

      mycircle->toggleDefaultVisualizer();
      moveCircleToCurve(mycircle);

      mycircle->setMaterial(GMlib::GMcolor::crimson());
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

void MyVisualizer::localSimulate(double dt)
{
  for (auto* circle : this->_circles) {
    moveCircleToCurve(circle);
  }
  //TODO: Implement
}

void MyVisualizer::moveCircleToCurve(MyVisualizerCircle<float>* circle)
{

  float t = circle->getT();
  GMVec3 coord = this->_curve->getPosition(t);
  GMVec3 der1 = this->_curve->getDer1(t);

    circle->set({ coord[0], coord[1], coord[2] },
                {  der1[0],  der1[1],  der1[2] },
                {        1,        0,        0 });
}

float MyVisualizer::calculateCurvature(float t)
{
  GMVec3 der1 = this->_curve->getDer1(t);
  GMVec3 der2 = this->_curve->getDer2(t);

  float divisor = (der1 ^ der2).getLength();
  double dividend = std::pow(der1.getLength(), 3);

  return divisor / float(dividend);
}

float MyVisualizer::calculateTorsion(float t)
{
  GMVec3 der1 = this->_curve->getDer1(t);
  GMVec3 der2 = this->_curve->getDer2(t);
  GMVec3 der3 = this->_curve->getDer2(t);

  float divisor = (der1 ^ der2) * der3;
  double dividend = std::pow((der1 ^ der2).getLength(), 2);

  return divisor / float(dividend);
}

} // namespace MySoothingNamespace

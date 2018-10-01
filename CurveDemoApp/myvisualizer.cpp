#include "myvisualizer.h"
#include "gmParametricsModule"
#include "myviscircle.h"

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


      auto* mycircle = new MyVisualizerCircle<float>(t, 0.25);

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
  for(auto* circle : this->_circles) {
    moveCircleToCurve(circle);
  }
  //TODO: Implement
}

void MyVisualizer::moveCircleToCurve(MyVisualizerCircle<float> *circle)
{
  auto t = circle->getT();
  auto coord = this->_curve->getPosition(t);
  auto der1 = this->_curve->getDer1(t);

  circle->set({ coord[0], coord[1], coord[2] },
              {  der1[0],  der1[1],  der1[2] },
              {        1,        0,        0 });
}

} // namespace MySoothingNamespace

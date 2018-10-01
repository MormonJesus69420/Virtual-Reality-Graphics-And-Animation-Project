#include "myvisualizer.h"
#include "gmParametricsModule"

namespace MySoothingNamespace {
MyVisualizer::MyVisualizer(GMlib::PBSplineCurve<float>* c, int sampleSize)
{

  wrapper = &(GMlibWrapper::instance());
  if (wrapper == nullptr) {
    //If this happens, we tried to get the wrapper too early
    throw std::runtime_error("Wrapper should not be nullptr, check if the visualizer was constructed too early");
  }
  this->curve = c;
  this->samples = sampleSize;
  this->circles = GMlib::Array<GMlib::PCircle<float>*>();
}

bool MyVisualizer::visualize()
{
  try {
    for (float t = 0; t <= this->curve->getParEnd(); t += 0.05) {
      auto coord = this->curve->getPosition(t);
      auto der1 = this->curve->getDer1(t);
      std::cout << coord << std::endl;
      auto mycircle = new GMlib::PCircle<float>(/*myBSpline->getCurvature(t)/5*/ 0.25);
      mycircle->toggleDefaultVisualizer();
      mycircle->set({ coord[0], coord[1], coord[2] },
          { der1[0], der1[1], der1[2] },
          { 1, 0, 0 });
      mycircle->setMaterial(GMlib::GMcolor::crimson());
      mycircle->sample(100, 2);
      this->circles.insert(mycircle);
      this->wrapper->scene()->insert(mycircle);
    }
    return true;
  }
  catch (std::exception e) {
    std::cerr << e.what();
    return false;
  }

  //TODO: Visualize, adapt, overcome
}

void MyVisualizer::update(double dt)
{
  this->localSimulate(dt);
}

void MyVisualizer::localSimulate(double dt)
{
  //TODO: Implement
}

} // namespace MySoothingNamespace

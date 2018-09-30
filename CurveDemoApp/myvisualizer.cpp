#include "myvisualizer.h"
#include "gmParametricsModule"

template <typename T>
MyVisualizer<T>::MyVisualizer(GMlib::PBSplineCurve<T>* c, int sampleSize)
{

  wrapper = &(GMlibWrapper::instance());
  if (wrapper == nullptr) {
    //If this happens, we tried to get the wrapper too early
    throw std::runtime_error("Wrapper should not be nullptr, check if the visualizer was constructed too early");
  }
  this->curve = c;
  this->samples = sampleSize;
}

template <typename T>
bool MyVisualizer<T>::visualize()
{
  //TODO: Visualize, adapt, overcome
}

template <typename T>
void MyVisualizer<T>::update(double dt)
{
}

template <typename T>
void MyVisualizer<T>::localSimulate(double dt)
{
  //TODO: Implement
}

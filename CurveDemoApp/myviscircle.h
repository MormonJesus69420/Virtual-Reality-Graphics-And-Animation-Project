#ifndef MY_VISUALIZE_CIRCLE
#define MY_VISUALIZE_CIRCLE
#include "gmParametricsModule"

namespace MySoothingNamespace {

template <typename T>
class MyVisualizerCircle : public GMlib::PCircle<T> {
  public:
  MyVisualizerCircle<T>(float t, T radius)
      : GMlib::PCircle<T>(radius)
  {
    this->_t = t;
  }

  float getT()
  {
    return this->_t;
  }

  private:
  float _t;
};

} // namespace MySoothingNamespace

#endif

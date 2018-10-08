#ifndef MY_VISUALIZE_CIRCLE
#define MY_VISUALIZE_CIRCLE
#include "gmParametricsModule"

namespace MySoothingNamespace {

template <typename T>
class MyVisualizerCircle : public GMlib::PCircle<T> {
  public:
  MyVisualizerCircle<T>(T radius)
      : GMlib::PCircle<T>(radius)
  {
  }
};

} // namespace MySoothingNamespace

#endif

#include "application/gmlibwrapper.h"

// Forward declarations
namespace GMlib {
template <typename T>
class PBSplineCurve;

} // namespace GMlib

/*
 * @TODO: Implement this class, obviously
 */
namespace MySoothingNamespace {
template <typename T>
class MyVisualizerCircle;

class MyVisualizer {
  public:
  MyVisualizer(GMlib::PBSplineCurve<float>* c, int sampleSize);
  bool visualize();
  void update(double dt);

  protected:
  void localSimulate(double dt);

  private:
  std::vector<MyVisualizerCircle<float>*> _circles;
  GMlib::PBSplineCurve<float>* _curve;
  GMlibWrapper* _wrapper = nullptr;
  int _samples;

  void moveCircleToCurve(MyVisualizerCircle<float>* circle);
};

} // namespace MySoothingNamespace

#include "application/gmlibwrapper.h"
#include <math.h>

// Forward declarations
namespace GMlib {
template <typename T>
class PBSplineCurve;
class SceneObject;

} // namespace GMlib

namespace MySoothingNamespace {

template <typename T>
class MyVisualizerCircle;

class MyVisualizer { //: public GMlib::SceneObject {
  public:
  MyVisualizer(GMlib::PBSplineCurve<float>* c, int sampleSize);
  bool visualize();
  void update(double dt);

  protected:
  void localSimulate(double dt); //override;

  private:
  std::vector<MyVisualizerCircle<float>*> _circles;
  GMlib::PBSplineCurve<float>* _curve;
  GMlibWrapper* _wrapper = nullptr;
  int _samples;

  void moveCircleToCurve(MyVisualizerCircle<float>* circle);
  float calculateCurvature(float t);
  float calculateTorsion(float t);
};

} // namespace MySoothingNamespace

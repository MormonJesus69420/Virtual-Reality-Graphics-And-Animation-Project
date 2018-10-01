#include "application/gmlibwrapper.h"

// Forward declarations
namespace GMlib {
template <typename T>
class PBSplineCurve;

template <typename T>
class PCircle;
} // namespace GMlib

/*
 * @TODO: Implement this class, obviously
 */
namespace MySoothingNamespace {

class MyVisualizer {
  public:
  MyVisualizer(GMlib::PBSplineCurve<float>* c, int sampleSize);
  bool visualize();
  void update(double dt);

  protected:
  void localSimulate(double dt);

  private:
  GMlib::Array<GMlib::PCircle<float>*> _circles;
  GMlib::PBSplineCurve<float>* _curve;
  GMlibWrapper* _wrapper = nullptr;
  int _samples;
};

} // namespace MySoothingNamespace

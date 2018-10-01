#include "application/gmlibwrapper.h"

//Forward declaration(s)
namespace GMlib {
template <typename T>
class PBSplineCurve;
}

/*
 * @TODO: Implement this class, obviously
 * */
namespace MySoothingNamespace {

class MyVisualizer {
  public:
  MyVisualizer(GMlib::PBSplineCurve<float>* c, int sampleSize);
  bool visualize();
  void update(double dt);

  protected:
  void localSimulate(double dt);

  private:
  GMlibWrapper* wrapper = nullptr;
  GMlib::PBSplineCurve<float>* curve;
  int samples;
};

} // namespace MySoothingNamespace

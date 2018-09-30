#include "application/gmlibwrapper.h"

//Forward declaration(s)
namespace GMlib {
template <typename T>
class PBSplineCurve;
}

/*
 * @TODO: Implement this class, obviously
 * */
template <typename T>
class MyVisualizer {
  public:
  MyVisualizer(GMlib::PBSplineCurve<T>* c, int sampleSize);
  bool visualize();
  void update(double dt);

  protected:
  void localSimulate(double dt);

  private:
  GMlibWrapper* wrapper = nullptr;
  GMlib::PBSplineCurve<T>* curve;
  int samples;
};

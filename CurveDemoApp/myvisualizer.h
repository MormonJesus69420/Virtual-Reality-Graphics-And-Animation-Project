#include "application/gmlibwrapper.h"

//stl
#include <math.h>

// Forward declarations
namespace GMlib {
template <typename T>
class PBSplineCurve;
class SceneObject;

} // namespace GMlib

namespace MySoothingNamespace {

using GMVec3 = GMlib::Vector<float, 3>;

template <typename T>
class MyVisualizerCircle;

class MyVisualizer { //: public GMlib::SceneObject {
  public:
  MyVisualizer(const GMlib::PBSplineCurve<float>* c, const int sampleSize, const float tStep = 0.01f);
  bool visualize();
  void update(const double dt);

  float findGreatestCurvature() const;
  protected:
  void localSimulate(const double dt); //override;

  private:
  //TODO: Find better name
  struct CurveParams{
    CurveParams(){}
    CurveParams(const CurveParams& p);
    CurveParams(CurveParams && p);
    CurveParams(float t, float curv, float to, const GMVec3& pos, const GMVec3& d1);

    CurveParams operator=(const CurveParams& p);

    float t;
    float curvature;
    float torsion;
    GMVec3 pos;
    GMVec3 der1;
    MyVisualizerCircle<float>* circle = nullptr;
  };

  std::vector<MyVisualizerCircle<float>*> _circles;
  std::vector<CurveParams> _params;
  const GMlib::PBSplineCurve<float>* _curve;
  GMlibWrapper* _wrapper = nullptr;
  int _samples;
  float _tStep;

  void setupParams();
  void setupParam(const float t);
  void updateParams();
  void updateParam(CurveParams& p);
  void moveCircleToCurve(CurveParams& p);
  float calculateCurvature(float t) const;
  float calculateTorsion(float t) const;



};

} // namespace MySoothingNamespace

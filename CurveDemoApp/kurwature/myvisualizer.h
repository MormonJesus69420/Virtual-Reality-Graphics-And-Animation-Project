#include "../application/gmlibwrapper.h"

//GMlib
#include "gmCoreModule"

//stl
#include <math.h>

// Forward declarations
namespace GMlib {
template <typename T>
class PBSplineCurve;
class SceneObject;
template <typename T>
class PCircle;
} // namespace GMlib

namespace MySoothingNamespace {

using GMVec3 = GMlib::Vector<float, 3>;

class MyBSplineCurve;

template <typename T>
class MyVisualizerCircle;

class MyVisualizer {
  friend MyBSplineCurve;

  public:
  MyVisualizer(const GMlib::PBSplineCurve<float>* c, const float tStep = .005f);
  MyVisualizer(const MyVisualizer& vis, const GMlib::PBSplineCurve<float>* c);
  bool visualize();
  void updateParams();
  void cleanup();

  void removeCircles();
  void insertCircles();

  private:
  struct CurveParams {
    CurveParams() {}
    CurveParams(const CurveParams& p) = default;
    CurveParams(CurveParams&& p);
    CurveParams(float t, float curvature, float torsion, const GMVec3& position, const GMVec3& tangent);

    CurveParams& operator=(const CurveParams& p) = default;
    CurveParams& operator=(CurveParams&& p);

    float t, curvature, torsion;
    GMVec3 position, tangent;
    std::shared_ptr<GMlib::PCircle<float>> circle;
  };

  const GMlib::PBSplineCurve<float>* _curve;
  std::vector<CurveParams> _params;
  GMlibWrapper* _wrapper = nullptr;
  GMVec3 _up_vector = { 1, 0, 0 };
  float _tStep, _greatest_torsion;

  void setupParams();
  void setupCircles();
  void setupParam(const float t);
  void updateParam(CurveParams& p);
  void moveCircleToCurve(CurveParams& p);
  void findGreatestTorsion();
  float calculateTorsion(float t) const;
  GMlib::Color calculateColor(const CurveParams& p);
};

} // namespace MySoothingNamespace

#ifndef SCENARIO_H
#define SCENARIO_H

#include "application/gmlibwrapper.h"

// qt
#include <QObject>

//Forward declarations
namespace GMlib {
template<typename T, int n>
class PVolumeDefaultVisualizer;

}
class TerrainVolume;



namespace MySoothingNamespace {
class MyBSplineCurve;

class Scenario : public GMlibWrapper {
  Q_OBJECT
  public:
  using GMlibWrapper::GMlibWrapper;

  void initializeScenario() override;
  void cleanupScenario() override;

  public slots:
  void toggleSimulation();
  void callDefferedGL();

  private:
  bool curve_sim_on = true;


  std::shared_ptr<MyBSplineCurve> _curve;
  std::shared_ptr<TerrainVolume> _tv;
  void initCurve();
  void initVolumetric();
  std::shared_ptr<std::vector<GMlib::Point<float,3>>> readFile(const std::string& fileName) const;
  void readData(std::vector<GMlib::Vector<float,3>> data);
  std::shared_ptr<GMlib::PVolumeDefaultVisualizer<float, 3>> _pvdv;
};

} // namespace MySoothingNamespace
#endif // SCENARIO_H

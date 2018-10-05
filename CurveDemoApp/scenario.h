#ifndef SCENARIO_H
#define SCENARIO_H

#include "application/gmlibwrapper.h"

// qt
#include <QObject>

namespace MySoothingNamespace {

class Scenario : public GMlibWrapper {
  Q_OBJECT
  public:
  using GMlibWrapper::GMlibWrapper;

  void initializeScenario() override;
  void cleanupScenario() override;

  void initCurve();

  public slots:
  void callDefferedGL();

  private:
};

} // namespace MySoothingNamespace
#endif // SCENARIO_H

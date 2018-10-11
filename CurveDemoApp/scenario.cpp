#include "scenario.h"

#include "kurwature/mybsplinecurve.h"
#include "kurwature/myvisualizer.h"
#include "volumeshite/terrainvolume.h"

//Alexander sin kode
//// hidmanager
//#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <gmCoreModule>
#include <gmOpenglModule>
#include <gmParametricsModule>
#include <gmSceneModule>

// qt
#include <QQuickItem>

//#include <iostream>
//#include <fstream>
//#include <sstream>

namespace MySoothingNamespace {

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
{
  out << v.size() << std::endl;
  for (uint i = 0; i < v.size(); i++)
    out << " " << v[i];
  out << std::endl;
  return out;
}

void Scenario::initializeScenario()
{
  // Insert a light
  GMlib::Point<GLfloat, 3> init_light_pos(2.0, 4.0, 10);
  GMlib::PointLight* light = new GMlib::PointLight(GMlib::GMcolor::white(), GMlib::GMcolor::white(),
      GMlib::GMcolor::white(), init_light_pos);
  light->setAttenuation(0.8f, 0.002f, 0.0008f);
  this->scene()->insertLight(light, false);

  // Insert Sun
  this->scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float, 3> init_cam_pos(0.0f, 0.0f, 0.0f);
  GMlib::Vector<float, 3> init_cam_dir(0.0f, 1.0f, 0.0f);
  GMlib::Vector<float, 3> init_cam_up(0.0f, 0.0f, 1.0f);

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos, init_cam_dir, init_cam_up);
  proj_rcpair.camera->setCuttingPlanes(1.0f, 8000.0f);
  proj_rcpair.camera->rotateGlobal(GMlib::Angle(-45), GMlib::Vector<float, 3>(1.0f, 0.0f, 0.0f));
  proj_rcpair.camera->translateGlobal(GMlib::Vector<float, 3>(0.0f, -20.0f, 20.0f));
  scene()->insertCamera(proj_rcpair.camera.get());
  proj_rcpair.renderer->reshape(GMlib::Vector<int, 2>(init_viewport_size, init_viewport_size));

  // Front cam
  auto front_rcpair = createRCPair("Front");
  front_rcpair.camera->set(init_cam_pos + GMlib::Vector<float, 3>(0.0f, -50.0f, 0.0f), init_cam_dir, init_cam_up);
  front_rcpair.camera->setCuttingPlanes(1.0f, 8000.0f);
  scene()->insertCamera(front_rcpair.camera.get());
  front_rcpair.renderer->reshape(GMlib::Vector<int, 2>(init_viewport_size, init_viewport_size));

  // Side cam
  auto side_rcpair = createRCPair("Side");
  side_rcpair.camera->set(init_cam_pos + GMlib::Vector<float, 3>(-50.0f, 0.0f, 0.0f), GMlib::Vector<float, 3>(1.0f, 0.0f, 0.0f), init_cam_up);
  side_rcpair.camera->setCuttingPlanes(1.0f, 8000.0f);
  scene()->insertCamera(side_rcpair.camera.get());
  side_rcpair.renderer->reshape(GMlib::Vector<int, 2>(init_viewport_size, init_viewport_size));

  // Top cam
  auto top_rcpair = createRCPair("Top");

  top_rcpair.camera->set(init_cam_pos + GMlib::Vector<float, 3>(0.0f, 0.0f, 50.0f), -init_cam_up, init_cam_dir);
  top_rcpair.camera->setCuttingPlanes(1.0f, 8000.0f);
  scene()->insertCamera(top_rcpair.camera.get());
  top_rcpair.renderer->reshape(GMlib::Vector<int, 2>(init_viewport_size, init_viewport_size));

  initCurve();
  initVolumetric();

  scene()->insert(_curve.get());
}

void Scenario::toggleSimulation()
{
  cleanupScenario();

  curve_sim_on = !curve_sim_on;

  if (curve_sim_on) {
    scene()->insert(_curve.get());
    _curve->getVisualizer()->insertCircles();
  }
  else {
    scene()->insert(_tv.get());
  }
}

void Scenario::cleanupScenario()
{
  if (curve_sim_on) {
    scene()->remove(_curve.get());
    _curve->getVisualizer()->removeCircles();
  }
  else {
    scene()->remove(_tv.get());
  }
}

void Scenario::initCurve()
{
  GMlib::DVector<GMlib::Vector<float, 3>> cp(8);
  cp[0] = GMlib::Vector<float, 3>(0, 0, 0);
  cp[1] = GMlib::Vector<float, 3>(1, 1, 0);
  cp[2] = GMlib::Vector<float, 3>(2, 0, 2);
  cp[3] = GMlib::Vector<float, 3>(3, 2, 0);
  cp[4] = GMlib::Vector<float, 3>(4, 1, 0);
  cp[5] = GMlib::Vector<float, 3>(5, 1, -2);
  cp[6] = GMlib::Vector<float, 3>(6, 2, 0);
  cp[7] = GMlib::Vector<float, 3>(7, 0, 0);
  _curve = std::make_shared<MyBSplineCurve>(cp, 3, false);
  _curve->toggleDefaultVisualizer();
  _curve->setColor(GMlib::Color(0, 0, 0, 0)); //This color is better than maroon
  _curve->showSelectors(0.5);
  _curve->sample(100, 4);
}

void Scenario::initVolumetric()
{
  std::shared_ptr<std::vector<GMlib::Point<float, 3>>> content = readFile("bjerkvikground.txt");
  _tv = std::make_shared<TerrainVolume>(GMlib::Vector<int, 3>(DIM, DIM, DIM), content);
  _pvdv = std::make_shared<GMlib::PVolumeDefaultVisualizer<float, 3>>();
  //pvdv->setSlicingVector(0.5, 0, 0.0);
  //pvdv->setShaders(false, false, false, false, false, true);
  _pvdv->updateTransferValues(false);

  _tv->insertVisualizer(_pvdv.get());
  _tv->replot(DIM, DIM, DIM, 0, 0, 0);
}

std::shared_ptr<std::vector<GMlib::Point<float, 3>>> Scenario::readFile(const std::string& fileName) const
{
  std::ifstream file;
  std::shared_ptr<std::vector<GMlib::Point<float, 3>>> result = std::make_shared<std::vector<GMlib::Point<float, 3>>>();
  result->reserve(2243319);
  try {
    file.open(fileName);
    float x, y, z;
    while (file >> x >> y >> z) {
      result->emplace(result->end(), x, y, z);
    }
  }
  catch (std::runtime_error e) {
    std::cerr << e.what() << std::endl;
  }
  catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }

  return result;
}

void Scenario::pointClicked(int x, int y, int box) {
  _cdata.push_back({x,y,box});
}
void Scenario::pointsClear() {
  _pvdv->getTransferFunction()->clearPoints();
}

void Scenario::callDefferedGL()
{
  GMlib::Array<const GMlib::SceneObject*> e_obj;
  this->scene()->getEditedObjects(e_obj);

  for (int i = 0; i < e_obj.getSize(); i++)
    if (e_obj(i)->isVisible())
      e_obj(i)->replot();

  while(!_cdata.empty()) {
    const ColorInsertData& theD = _cdata.front();
    _pvdv->getTransferFunction()->insertPoint(theD.x, theD.y, 100, 100, theD.type, true);
    _cdata.pop_front();
  }
  _pvdv->updateTransferValues(true);
}

} // namespace MySoothingNamespace

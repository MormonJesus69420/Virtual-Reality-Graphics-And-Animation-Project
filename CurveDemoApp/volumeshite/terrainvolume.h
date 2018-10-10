#ifndef TERRAINVOLUME_H
#define TERRAINVOLUME_H

//#include "../gmpfinitedifferencevolume.h"

#include <gmParametricsModule>

#include "gmpfinitedifferencevolume.h"

//stl
#include <memory>

#define DIM 50

class TerrainVolume : public GMlib::PFiniteDifferenceVolume<float, 3> {
  public:
  TerrainVolume(GMlib::Vector<int, 3> dim);
  TerrainVolume(GMlib::Vector<int, 3> dim, std::shared_ptr<std::vector<GMlib::Point<float,3>>> tData);


  void toggleNextStep();

  protected:
  void localSimulate(double dt);

  private:
  float getValue(GMlib::Vector<float, 3> xyz);
  GMlib::Vector<int, 3> _dim;
  int step;
  GMlib::DVectorN<GMlib::CFDCell<float, 3>, 3> data;
  GMlib::Vector<float,3> _delta;
  GMlib::Vector<float,3> _min;
  GMlib::Vector<float,3> _max;
  int _maxPoints = 0;
  std::shared_ptr<std::vector<GMlib::Vector<int,3>>> _transformed;
  std::shared_ptr<GMlib::Vector<GMlib::Matrix<int, DIM, DIM>, DIM>> _points;

  GMlib::Vector<float, 3> computeMaxValues(std::vector<GMlib::Point<float,3>>* tData);
  GMlib::Vector<float, 3> computeMinValues(std::vector<GMlib::Point<float,3>>* tData);
  float convertToHeatColors(float currentTemperature, float min, float max);
};

#endif // TERRAINVOLUME_H

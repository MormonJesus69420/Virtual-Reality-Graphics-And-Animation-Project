#ifndef TERRAINVOLUME_H
#define TERRAINVOLUME_H

//#include "../gmpfinitedifferencevolume.h"

#include <gmParametricsModule>

#include "gmpfinitedifferencevolume.h"


class TerrainVolume : public GMlib::PFiniteDifferenceVolume<float,3>
  {
public:
  TerrainVolume(GMlib::Vector<int,3> dim);

  void  toggleNextStep();

protected:
  void localSimulate(double dt);

private:

  float getValue(GMlib::Vector<float,3> xyz);
  GMlib::Vector<int,3> _dim;
  int step;
  GMlib::DVectorN<GMlib::CFDCell<float,3> ,3 > data;

  float convertToHeatColors(float currentTemperature, float min, float max);

};

#endif // TERRAINVOLUME_H

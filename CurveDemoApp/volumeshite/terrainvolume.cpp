#include "terrainvolume.h"
#include "gmpfinitedifferencevolume.h"
#include "transferfunction.h"
#include <QDebug>
#include <cmath>
#include <random>

TerrainVolume::TerrainVolume(GMlib::Vector<int, 3> dim)
    : GMlib::PFiniteDifferenceVolume<float, 3>()
{
  _dim = dim;
  data = GMlib::DVectorN<GMlib::CFDCell<float, 3>, 3>();
  data.setDim(_dim);

  float min = getValue(GMlib::Vector<float, 3>(2, 2, 2));
  float max = getValue(GMlib::Vector<float, 3>(_dim[0] - 2, _dim[1] - 2, _dim[2] - 2));

  for (int i = 0; i < _dim[0]; i++)
    for (int j = 0; j < _dim[1]; j++)
      for (int k = 0; k < _dim[2]; k++) {
        data[i][j][k][0] = i;
        data[i][j][k][1] = j;
        data[i][j][k][2] = k;

        float value = getValue(GMlib::Vector<float, 3>(i, j, k));
        float hvalue = convertToHeatColors(value, min, max);
        data[i][j][k].setColor(GMlib::Vector<float, 4>(hvalue, 1 - hvalue, 0, 1));
      }

  //    data[0][0][0].setColor(GMlib::Vector<float,4>(1,0,0,1));
  this->setPointSet(data, 0);
  step = 0;
}

TerrainVolume::TerrainVolume(GMlib::Vector<int, 3> dim, std::shared_ptr<std::vector<GMlib::Point<float, 3>>> tData)
{
  _dim = dim;
  data = GMlib::DVectorN<GMlib::CFDCell<float, 3>, 3>();
  data.setDim(_dim);

  _max = computeMaxValues(tData.get());
  _min = computeMinValues(tData.get());
  _delta = _max - _min;

  _transformed = std::make_shared<std::vector<GMlib::Vector<int, 3>>>();


  std::transform(tData->begin(), tData->end(), std::back_insert_iterator<std::vector<GMlib::Vector<int, 3>>>(*_transformed),
      [this](GMlib::Point<float, 3>& vec) {
        int x = int(std::lround(((vec[0] - this->_min[0]) / this->_delta[0]) * this->_dim[0]));
        int y = int(std::lround(((vec[1] - this->_min[1]) / this->_delta[1]) * this->_dim[1]));
        int z = int(std::lround(((vec[2] - this->_min[2]) / this->_delta[2]) * this->_dim[2]));
//        int z = int(std::lround(vec[2]));
//        z++;
        if(x == _dim[0])
          x--;
        if(y == _dim[1])
          y--;
        if(z == _dim[2])
          z--;
        return GMlib::Vector<int, 3>{ z,y,x };
      });

  _points = std::make_shared<GMlib::Vector<GMlib::Matrix<int, DIM, DIM>, DIM>>();

  for (const auto& vec : *_transformed) {
    (*_points)[vec[0]][vec[1]][vec[2]]++;

    if (_maxPoints < (*_points)[vec[0]][vec[1]][vec[2]])
      _maxPoints = (*_points)[vec[0]][vec[1]][vec[2]];
  }

  int colMax;
  for(int i = 0; i < DIM; ++i){
    for(int j = 0; j < DIM; ++j){
      colMax = 0;
      for(int k = 0; k < DIM; ++k){
        if((*_points)[k][i][j] > (*_points)[colMax][i][j])
          colMax = k;
      }
      auto maxval = (*_points)[colMax][i][j];
      for(int k = 0; k < colMax; ++k)
        (*_points)[k][i][j] = maxval;
    }
  }

  for (int i = 0; i < _dim[0]; i++)
    for (int j = 0; j < _dim[1]; j++)
      for (int k = 0; k < _dim[2]; k++) {
        data[i][j][k][0] = i;
        data[i][j][k][1] = j;
        data[i][j][k][2] = k;

        //        float value = getValue(GMlib::Vector<float, 3>(i, j, k));
        float value = float(((*_points)[i][j][k]));
        float hvalue = convertToHeatColors(value, 0.f, _maxPoints);
        data[i][j][k].setColor(GMlib::Vector<float, 4>(hvalue, 1 - hvalue, 0, 1));
      }

  //    data[0][0][0].setColor(GMlib::Vector<float,4>(1,0,0,1));
  this->setPointSet(data, 0);
  step = 0;
}

float TerrainVolume::getValue(GMlib::Vector<float, 3> xyz)
{
  float x = xyz[0];
  float y = xyz[1];
  float z = xyz[2];

  if (x + y + z <= 0)
    return 0;

  //    float tval = x+y+z;
  float tval = x * x + y * y + z * z;
  //    float tval = x+y+z;

  return tval;
}

//Begin anonymous namespace
namespace {
auto compareX = [](const GMlib::Vector<float, 3>& a, const GMlib::Vector<float, 3>& b) {
  return a[0] < b[0];
};

auto compareY = [](const GMlib::Vector<float, 3>& a, const GMlib::Vector<float, 3>& b) {
  return a[1] < b[1];
};

auto compareZ = [](const GMlib::Vector<float, 3>& a, const GMlib::Vector<float, 3>& b) {
  return a[2] < b[2];
};

} //namespace

GMlib::Vector<float, 3> TerrainVolume::computeMaxValues(std::vector<GMlib::Point<float, 3>>* tData)
{
  auto xIt = std::max_element(tData->begin(), tData->end(), compareX);

  auto yIt = std::max_element(tData->begin(), tData->end(), compareY);

  auto zIt = std::max_element(tData->begin(), tData->end(), compareZ);

  return { (*xIt)[0], (*yIt)[1], (*zIt)[2] };
}

GMlib::Vector<float, 3> TerrainVolume::computeMinValues(std::vector<GMlib::Point<float, 3>>* tData)
{
  auto xIt = std::min_element(tData->begin(), tData->end(), compareX);

  auto yIt = std::min_element(tData->begin(), tData->end(), compareY);

  auto zIt = std::min_element(tData->begin(), tData->end(), compareZ);

  return { (*xIt)[0], (*yIt)[1], (*zIt)[2] };
}

void TerrainVolume::toggleNextStep()
{

  std::cout << "toggle next step terrain volume" << std::endl;
  //    GMlib::DVectorN<GMlib::CFDCell<float,3> ,3 > pointset;
  //    pointset.setDim(GMlib::Vector<int,3>(_dim[0],_dim[1],_dim[2]));
  float min = getValue(GMlib::Vector<float, 3>(0, 0, 0));
  float max [[maybe_unused]] = getValue(GMlib::Vector<float, 3>(_dim[0], _dim[1], _dim[2]));

  for (int i = 0; i < _dim[0]; i++)
    for (int j = 0; j < _dim[1]; j++)
      for (int k = 0; k < _dim[2]; k++) {
        float randomaplha = rand() / double(RAND_MAX);
        auto rnd1 = rand() % _dim[0];
        auto rnd2 = rand() % _dim[1];
        auto rnd3 = rand() % _dim[2];
        float v1 = getValue(GMlib::Vector<float, 3>(rnd1, 0, 0));
        float v2 = getValue(GMlib::Vector<float, 3>(0, rnd2, 0));
        float v3 = getValue(GMlib::Vector<float, 3>(0, 0, rnd3));
        float r = convertToHeatColors(v1, min, _maxPoints);
        float g = convertToHeatColors(v2, min, _maxPoints);
        float b = convertToHeatColors(v3, min, _maxPoints);
        data[i][j][k].setColor(GMlib::Vector<float, 4>(r, g, b, randomaplha));
        if (i > 8 && i < 22 && j > 8 && j < 22)
          data[i][j][k].setColor(GMlib::Vector<float, 4>(1, 1, 1, 1));
        if (i > 8 && i < 22 && k > 8 && k < 22)
          data[i][j][k].setColor(GMlib::Vector<float, 4>(1, 1, 1, 1));
        if (k > 8 && k < 22 && j > 8 && j < 22)
          data[i][j][k].setColor(GMlib::Vector<float, 4>(1, 1, 1, 1));
        if (k > 13 && k < 16 && j > 13 && j < 16 && i > 13 && i < 16)
          data[i][j][k].setColor(GMlib::Vector<float, 4>(1, 0, 0, 1));
      }
  //    int randoms = 20000;

  //    for(int i = 0; i < randoms; ++i){
  //        float randomaplha = rand() / double(RAND_MAX);
  //        data[rand()%_dim[0]][rand()%_dim[0]][rand()%_dim[0]].setColor(GMlib::Vector<float,4>(1,1,1,randomaplha));
  //    }

  //    for(int i = 0; i < _dim[2]; ++i)
  //        data[_dim[0]/2][_dim[1]/2][i].setColor(GMlib::Vector<float,4>(1,1,1,1));

  this->setPointSet(data, 0);
  step++;
}

void TerrainVolume::localSimulate(double dt)
{
}

float TerrainVolume::convertToHeatColors(float currentTemperature, float min, float max)
{
  float minInit = min;
  float maxInit = max;

  float delta = (maxInit - minInit);

  float retVal = (currentTemperature - minInit) / delta;

  //    qDebug() << "Return: " << retVal;

  return retVal;
}

#include "gmpfinitedifferencevolume.h"

namespace GMlib {

template <typename T, int n, int m>
PFiniteDifferenceVolume<T, n, m>::PFiniteDifferenceVolume()
{
  _pSet = GMlib::DVectorN<GMlib::CFDCell<float, 3>, 3>();
  _order = 0;
}

template <typename T, int n, int m>
PFiniteDifferenceVolume<T, n, m>::PFiniteDifferenceVolume(Vector<int, 3> dim, int order)
    : PVolume<T, n>(dim[0], dim[1], dim[2])
{
  _pSet = GMlib::DVectorN<GMlib::CFDCell<float, 3>, 3>();
  _pSet.setDim(GMlib::Vector<int, 3>(dim[0], dim[1], dim[2]));

  for (int i = 0; i < dim[0]; i++)
    for (int j = 0; j < dim[1]; j++)
      for (int k = 0; k < dim[2]; k++) {
        _pSet[i][j][k][0] = i;
        _pSet[i][j][k][1] = j;
        _pSet[i][j][k][2] = k;
        _pSet[i][j][k].setColor(GMlib::Vector<float, 4>(0, 0, 0, 0));
      }

  _order = order;
  init();
}

template <typename T, int n, int m>
PFiniteDifferenceVolume<T, n, m>::PFiniteDifferenceVolume(DVectorN<CFDCell<T, n>, 3, T> c, int order)
    : PVolume<T, n>(c.getDim()[0], c.getDim()[1], c.getDim()[2])
{
  _pSet = c;
  _order = order;
  init();
}

template <typename T, int n, int m>
PFiniteDifferenceVolume<T, n, m>::~PFiniteDifferenceVolume()
{
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::setPointSet(DVectorN<CFDCell<T, n>, 3, T> c, int order)
{
  _pSet.setDim(GMlib::Vector<int, 3>(c.getDim()[0], c.getDim()[1], c.getDim()[2]));

  for (int i = 0; i < c.getDim()[0]; i++)
    for (int j = 0; j < c.getDim()[1]; j++)
      for (int k = 0; k < c.getDim()[2]; k++) {
        _pSet[i][j][k] = c[i][j][k];
      }

  //    _pSet = c;
  _order = order;
  init();
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::eval(T u, T v, T w, int d1, int d2, int d3, bool lu, bool lv, bool lw)
{
  int i = 0, j = 0, k = 0;
  this->findIndex(u, v, w, i, j, k);
  int _dim_[3] = { _order + 1, _order + 1, _order + 1 };
  this->_p.setDim(_dim_);

  //    float prosu = u/this->_no_sam_u;
  //    float prosv = v/this->_no_sam_v;
  //    float prosw = w/this->_no_sam_w;

  for (int p = 0; p < this->_p.getDim()[0]; p++)
    for (int l = 0; l < this->_p.getDim()[1]; l++)
      for (int o = 0; o < this->_p.getDim()[2]; o++) {
        //                std::cout << "Checking _p:" << this->_p[p][l][o] << std::endl;
        //                std::cout << "Checking _fSet:" << this->_fSet[i][j][k][p][l][o] << std::endl;
        this->_p[p][l][o] = _fSet[i][j][k][p][l][o];
      }
}

//  template <typename T, int n>
//  inline
//  void PVolume<T, n>::findIndex(T u, T v, T w, int &i, int &j, int &k)
//  {
//    T t = getParStartU();
//    T t2 = getParDeltaU();
//    T t3 = getParEndU();
//    i = ( _no_sam_u-1)*( u-getParStartU())/(getParDeltaU())+0.1;
//    j = ( _no_sam_v-1)*( v-getParStartV())/(getParDeltaV())+0.1;
//    k = ( _no_sam_w-1)*( w-getParStartW())/(getParDeltaW())+0.1;
//  }

template <typename T, int n, int m>
inline T PFiniteDifferenceVolume<T, n, m>::getEndPU()
{
  //    T val = _pSet[_pSet.getDim()[0]-1][0][0](0);
  return T(_pSet[_pSet.getDim()[0] - 1][0][0](0));
}

template <typename T, int n, int m>
inline T PFiniteDifferenceVolume<T, n, m>::getEndPV()
{
  //    T val = _pSet[0][_pSet.getDim()[1]-1][0](1);
  return T(_pSet[0][_pSet.getDim()[1] - 1][0](1));
}

template <typename T, int n, int m>
inline T PFiniteDifferenceVolume<T, n, m>::getEndPW()
{

  //    T val = _pSet[0][0][_pSet.getDim()[2]-1](2);
  return T(_pSet[0][0][_pSet.getDim()[2] - 1](2));
}

template <typename T, int n, int m>
inline T PFiniteDifferenceVolume<T, n, m>::getStartPU()
{
  //    T val = _pSet[0][0][0](0);
  return T(_pSet[0][0][0](0));
}

template <typename T, int n, int m>
inline T PFiniteDifferenceVolume<T, n, m>::getStartPV()
{
  //    T val = _pSet[0][0][0](1);
  return T(_pSet[0][0][0](1));
}

template <typename T, int n, int m>
inline T PFiniteDifferenceVolume<T, n, m>::getStartPW()
{
  //    T val = _pSet[0][0][0](2);
  return T(_pSet[0][0][0](2));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::forwardDiffU(int i, int j, int k, int d1, int d2, int d3)
{

  _fSet[i][j][k][d1][d2][d3] = ((Point<T, n>)((_fSet[i + 1][j][k][d1 - 1][d2][d3] - _fSet[i][j][k][d1 - 1][d2][d3]) / (_pSet[i + 1][j][k](0) - _pSet[i][j][k](0))));
  _fSet[i][j][k][d1][d2][d3].setColor((Vector<T, 4>)((_fSet[i + 1][j][k][d1 - 1][d2][d3].getColor() - _fSet[i][j][k][d1 - 1][d2][d3].getColor()) / ((_pSet[i + 1][j][k] - _pSet[i][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setDirection((Vector<T, 3>)((_fSet[i + 1][j][k][d1 - 1][d2][d3].getDirection() - _fSet[i][j][k][d1 - 1][d2][d3].getDirection()) / ((_pSet[i + 1][j][k] - _pSet[i][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setPos(((Point<T, 3>)((_fSet[i + 1][j][k][d1 - 1][d2][d3] - _fSet[i][j][k][d1 - 1][d2][d3]) / (_pSet[i + 1][j][k](0) - _pSet[i][j][k](0)))));
  _fSet[i][j][k][d1][d2][d3].setValue((T)((_fSet[i + 1][j][k][d1 - 1][d2][d3].getValue() - _fSet[i][j][k][d1 - 1][d2][d3].getValue()) / (_pSet[i + 1][j][k](0) - _pSet[i][j][k](0))));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::backwardDiffU(int i, int j, int k, int d1, int d2, int d3)
{
  _fSet[i][j][k][d1][d2][d3] = ((Point<T, n>)((_fSet[i][j][k][d1 - 1][d2][d3] - _fSet[i - 1][j][k][d1 - 1][d2][d3]) / (_pSet[i][j][k](0) - _pSet[i - 1][j][k](0))));
  _fSet[i][j][k][d1][d2][d3].setColor((Vector<T, 4>)((_fSet[i][j][k][d1 - 1][d2][d3].getColor() - _fSet[i - 1][j][k][d1 - 1][d2][d3].getColor()) / ((_pSet[i][j][k] - _pSet[i - 1][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setDirection((Vector<T, 3>)((_fSet[i][j][k][d1 - 1][d2][d3].getDirection() - _fSet[i - 1][j][k][d1 - 1][d2][d3].getDirection()) / ((_pSet[i][j][k] - _pSet[i - 1][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setPos((Point<T, 3>)((_fSet[i][j][k][d1 - 1][d2][d3].getPos() - _fSet[i - 1][j][k][d1 - 1][d2][d3].getPos()) / ((_pSet[i][j][k].getPos() - _pSet[i - 1][j][k].getPos()).getLength())));
  _fSet[i][j][k][d1][d2][d3].setValue((T)((_fSet[i][j][k][d1 - 1][d2][d3].getValue() - _fSet[i - 1][j][k][d1 - 1][d2][d3].getValue()) / (_pSet[i][j][k](0) - _pSet[i - 1][j][k](0))));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::symetricDiffU(int i, int j, int k, int d1, int d2, int d3)
{
  _fSet[i][j][k][d1][d2][d3] = ((Point<T, n>)((_fSet[i + 1][j][k][d1 - 1][d2][d3] - _fSet[i - 1][j][k][d1 - 1][d2][d3]) / (_pSet[i + 1][j][k](0) - _pSet[i - 1][j][k](0))));
  _fSet[i][j][k][d1][d2][d3].setColor((Vector<T, 4>)((_fSet[i + 1][j][k][d1 - 1][d2][d3].getColor() - _fSet[i - 1][j][k][d1 - 1][d2][d3].getColor()) / ((_pSet[i + 1][j][k] - _pSet[i - 1][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setDirection((Vector<T, 3>)((_fSet[i + 1][j][k][d1 - 1][d2][d3].getDirection() - _fSet[i - 1][j][k][d1 - 1][d2][d3].getDirection()) / ((_pSet[i + 1][j][k] - _pSet[i - 1][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setPos((Point<T, 3>)((_fSet[i + 1][j][k][d1 - 1][d2][d3].getPos() - _fSet[i - 1][j][k][d1 - 1][d2][d3].getPos()) / ((_pSet[i + 1][j][k].getPos() - _pSet[i - 1][j][k].getPos()).getLength())));
  _fSet[i][j][k][d1][d2][d3].setValue((T)((_fSet[i + 1][j][k][d1 - 1][d2][d3].getValue() - _fSet[i - 1][j][k][d1 - 1][d2][d3].getValue()) / (_pSet[i + 1][j][k](0) - _pSet[i - 1][j][k](0))));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::forwardDiffV(int i, int j, int k, int d1, int d2, int d3)
{
  _fSet[i][j][k][d1][d2][d3] = ((Point<T, n>)((_fSet[i][j + 1][k][d1][d2 - 1][d3] - _fSet[i][j][k][d1][d2 - 1][d3]) / (_pSet[i][j + 1][k](1) - _pSet[i][j][k](1))));
  _fSet[i][j][k][d1][d2][d3].setColor((Vector<T, 4>)((_fSet[i][j + 1][k][d1][d2 - 1][d3].getColor() - _fSet[i][j][k][d1][d2 - 1][d3].getColor()) / ((_pSet[i][j + 1][k] - _pSet[i][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setDirection((Vector<T, 3>)((_fSet[i][j + 1][k][d1][d2 - 1][d3].getDirection() - _fSet[i][j][k][d1][d2 - 1][d3].getDirection()) / ((_pSet[i][j + 1][k] - _pSet[i][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setPos((Point<T, 3>)((_fSet[i][j + 1][k][d1][d2 - 1][d3].getPos() - _fSet[i][j][k][d1][d2 - 1][d3].getPos()) / ((_pSet[i][j + 1][k].getPos() - _pSet[i][j][k].getPos()).getLength())));
  _fSet[i][j][k][d1][d2][d3].setValue((T)((_fSet[i][j + 1][k][d1][d2 - 1][d3].getValue() - _fSet[i][j][k][d1][d2 - 1][d3].getValue()) / (_pSet[i][j + 1][k](1) - _pSet[i][j][k](1))));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::backwardDiffV(int i, int j, int k, int d1, int d2, int d3)
{
  _fSet[i][j][k][d1][d2][d3] = ((Point<T, n>)((_fSet[i][j][k][d1][d2 - 1][d3] - _fSet[i][j - 1][k][d1][d2 - 1][d3]) / (_pSet[i][j][k](1) - _pSet[i][j - 1][k](1))));
  _fSet[i][j][k][d1][d2][d3].setColor((Vector<T, 4>)((_fSet[i][j][k][d1][d2 - 1][d3].getColor() - _fSet[i][j - 1][k][d1][d2 - 1][d3].getColor()) / ((_pSet[i][j][k] - _pSet[i][j - 1][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setDirection((Vector<T, 3>)((_fSet[i][j][k][d1][d2 - 1][d3].getDirection() - _fSet[i][j - 1][k][d1][d2 - 1][d3].getDirection()) / ((_pSet[i][j][k] - _pSet[i][j - 1][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setPos((Point<T, 3>)((_fSet[i][j][k][d1][d2 - 1][d3].getPos() - _fSet[i][j - 1][k][d1][d2 - 1][d3].getPos()) / ((_pSet[i][j][k].getPos() - _pSet[i][j - 1][k].getPos()).getLength())));
  _fSet[i][j][k][d1][d2][d3].setValue((T)((_fSet[i][j][k][d1][d2 - 1][d3].getValue() - _fSet[i][j - 1][k][d1][d2 - 1][d3].getValue()) / (_pSet[i][j][k](1) - _pSet[i][j - 1][k](1))));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::symetricDiffV(int i, int j, int k, int d1, int d2, int d3)
{
  _fSet[i][j][k][d1][d2][d3] = ((Point<T, n>)((_fSet[i][j + 1][k][d1][d2 - 1][d3] - _fSet[i][j - 1][k][d1][d2 - 1][d3]) / (_pSet[i][j + 1][k](1) - _pSet[i][j - 1][k](1))));
  _fSet[i][j][k][d1][d2][d3].setColor((Vector<T, 4>)((_fSet[i][j + 1][k][d1][d2 - 1][d3].getColor() - _fSet[i][j - 1][k][d1][d2 - 1][d3].getColor()) / ((_pSet[i][j + 1][k] - _pSet[i][j - 1][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setDirection((Vector<T, 3>)((_fSet[i][j + 1][k][d1][d2 - 1][d3].getDirection() - _fSet[i][j - 1][k][d1][d2 - 1][d3].getDirection()) / ((_pSet[i][j + 1][k] - _pSet[i][j - 1][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setPos((Point<T, 3>)((_fSet[i][j + 1][k][d1][d2 - 1][d3].getPos() - _fSet[i][j - 1][k][d1][d2 - 1][d3].getPos()) / ((_pSet[i][j + 1][k].getPos() - _pSet[i][j - 1][k].getPos()).getLength())));
  _fSet[i][j][k][d1][d2][d3].setValue((T)((_fSet[i][j + 1][k][d1][d2 - 1][d3].getValue() - _fSet[i][j - 1][k][d1][d2 - 1][d3].getValue()) / (_pSet[i][j + 1][k](1) - _pSet[i][j - 1][k](1))));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::forwardDiffW(int i, int j, int k, int d1, int d2, int d3)
{
  _fSet[i][j][k][d1][d2][d3] = ((Point<T, n>)((_fSet[i][j][k + 1][d1][d2][d3 - 1] - _fSet[i][j][k][d1][d2][d3 - 1]) / (_pSet[i][j][k + 1](2) - _pSet[i][j][k](2))));
  _fSet[i][j][k][d1][d2][d3].setColor((Vector<T, 4>)((_fSet[i][j][k + 1][d1][d2][d3 - 1].getColor() - _fSet[i][j][k][d1][d2][d3 - 1].getColor()) / ((_pSet[i][j][k + 1] - _pSet[i][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setDirection((Vector<T, 3>)((_fSet[i][j][k + 1][d1][d2][d3 - 1].getDirection() - _fSet[i][j][k][d1][d2][d3 - 1].getDirection()) / ((_pSet[i][j][k + 1] - _pSet[i][j][k]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setPos((Point<T, 3>)((_fSet[i][j][k + 1][d1][d2][d3 - 1].getPos() - _fSet[i][j][k][d1][d2][d3 - 1].getPos()) / ((_pSet[i][j][k + 1].getPos() - _pSet[i][j][k].getPos()).getLength())));
  _fSet[i][j][k][d1][d2][d3].setValue((T)((_fSet[i][j][k + 1][d1][d2][d3 - 1].getValue() - _fSet[i][j][k][d1][d2][d3 - 1].getValue()) / (_pSet[i][j][k + 1](2) - _pSet[i][j][k](2))));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::backwardDiffW(int i, int j, int k, int d1, int d2, int d3)
{
  _fSet[i][j][k][d1][d2][d3] = ((Point<T, n>)((_fSet[i][j][k][d1][d2][d3 - 1] - _fSet[i][j][k - 1][d1][d2][d3 - 1]) / ((_pSet[i][j][k](2) - _pSet[i][j][k - 1](2)))));
  _fSet[i][j][k][d1][d2][d3].setColor((Vector<T, 4>)((_fSet[i][j][k][d1][d2][d3 - 1].getColor() - _fSet[i][j][k - 1][d1][d2][d3 - 1].getColor()) / ((_pSet[i][j][k] - _pSet[i][j][k - 1]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setDirection((Vector<T, 3>)((_fSet[i][j][k][d1][d2][d3 - 1].getDirection() - _fSet[i][j][k - 1][d1][d2][d3 - 1].getDirection()) / ((_pSet[i][j][k] - _pSet[i][j][k - 1]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setPos((Point<T, 3>)((_fSet[i][j][k][d1][d2][d3 - 1].getPos() - _fSet[i][j][k - 1][d1][d2][d3 - 1].getPos()) / ((_pSet[i][j][k].getPos() - _pSet[i][j][k - 1].getPos()).getLength())));
  _fSet[i][j][k][d1][d2][d3].setValue((T)((_fSet[i][j][k][d1][d2][d3 - 1].getValue() - _fSet[i][j][k - 1][d1][d2][d3 - 1].getValue()) / ((_pSet[i][j][k](2) - _pSet[i][j][k - 1](2)))));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::symetricDiffW(int i, int j, int k, int d1, int d2, int d3)
{
  _fSet[i][j][k][d1][d2][d3] = ((Point<T, n>)((_fSet[i][j][k + 1][d1][d2][d3 - 1] - _fSet[i][j][k - 1][d1][d2][d3 - 1]) / (_pSet[i][j][k + 1](2) - _pSet[i][j][k - 1](2))));
  _fSet[i][j][k][d1][d2][d3].setColor((Vector<T, 4>)((_fSet[i][j][k + 1][d1][d2][d3 - 1].getColor() - _fSet[i][j][k - 1][d1][d2][d3 - 1].getColor()) / ((_pSet[i][j][k + 1] - _pSet[i][j][k - 1]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setDirection((Vector<T, 3>)((_fSet[i][j][k + 1][d1][d2][d3 - 1].getDirection() - _fSet[i][j][k - 1][d1][d2][d3 - 1].getDirection()) / ((_pSet[i][j][k + 1] - _pSet[i][j][k - 1]).getLength())));
  _fSet[i][j][k][d1][d2][d3].setPos((Vector<T, 3>)((_fSet[i][j][k + 1][d1][d2][d3 - 1].getPos() - _fSet[i][j][k - 1][d1][d2][d3 - 1].getPos()) / ((_pSet[i][j][k + 1].getPos() - _pSet[i][j][k - 1].getPos()).getLength())));
  _fSet[i][j][k][d1][d2][d3].setValue((T)((_fSet[i][j][k + 1][d1][d2][d3 - 1].getValue() - _fSet[i][j][k - 1][d1][d2][d3 - 1].getValue()) / (_pSet[i][j][k + 1](2) - _pSet[i][j][k - 1](2))));
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::replot(int m1, int m2, int m3, int d1, int d2, int d3)
{
  if (m1 > _pSet.getDim()[0])
    m1 = _pSet.getDim()[0];
  if (m2 > _pSet.getDim()[1])
    m2 = _pSet.getDim()[1];
  if (m3 > _pSet.getDim()[2])
    m3 = _pSet.getDim()[2];

  PVolume<T, n>::replot(m1, m2, m3, d1, d2, d3);
}

template <typename T, int n, int m>
inline void PFiniteDifferenceVolume<T, n, m>::init()
{
  _fSet.setDim(_pSet.getDim());
  int _size = (_order + 1);
  int i, j, k, d1, d2, d3;
  int _dim[3] = { _size, _size, _size };
  //    _xdif = (_pSet[_pSet.getDim()[0]-1][0][0]() - _pSet[0][0][0]()).length();
  //    _ydif = (_pSet[0][_pSet.getDim()[1]-1][0]() - _pSet[0][0][0]()).length();
  //    _zdif = (_pSet[0][0][_pSet.getDim()[2]-1]() - _pSet[0][0][0]()).length();

  for (d1 = 0; d1 < _size; d1++)
    for (d2 = 0; d2 < _size; d2++)
      for (d3 = 0; d3 < _size; d3++)
        for (i = 0; i < _pSet.getDim()[0]; i++)
          for (j = 0; j < _pSet.getDim()[1]; j++)
            for (k = 0; k < _pSet.getDim()[2]; k++) {
              if (d1 == 0 && d2 == 0 && d3 == 0) {
                _fSet[i][j][k].setDim(_dim);
                //                                std::cout << "ijk" << i << j << k << std::endl;
                _fSet[i][j][k][d1][d2][d3] = _pSet[i][j][k];
              }

              if (i == 0 && d1 > 0)
                forwardDiffU(i, j, k, d1, d2, d3);
              if (j == 0 && d2 > 0)
                forwardDiffV(i, j, k, d1, d2, d3);
              if (k == 0 && d3 > 0)
                forwardDiffW(i, j, k, d1, d2, d3);
              if (i == (_pSet.getDim()[0] - 1) && d1 > 0)
                backwardDiffU(i, j, k, d1, d2, d3);
              if (j == (_pSet.getDim()[1] - 1) && d2 > 0)
                backwardDiffV(i, j, k, d1, d2, d3);
              if (k == (_pSet.getDim()[2] - 1) && d3 > 0)
                backwardDiffW(i, j, k, d1, d2, d3);
              if (i > 0 && i < (_pSet.getDim()[0] - 1) && d1 > 0)
                symetricDiffU(i, j, k, d1, d2, d3);
              if (j > 0 && j < (_pSet.getDim()[1] - 1) && d2 > 0)
                symetricDiffV(i, j, k, d1, d2, d3);
              if (k > 0 && k < (_pSet.getDim()[2] - 1) && d3 > 0)
                symetricDiffW(i, j, k, d1, d2, d3);
              // Instead of 2h the symetric difference scheme uses the distance between
              // the points at +1 and -1 to compensate for a non uniform grid.
            }
}

} // namespace GMlib

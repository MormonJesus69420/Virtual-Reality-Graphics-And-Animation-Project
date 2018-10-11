#include "gmcfdcell.h"

template <class T, int n>
GMlib::CFDCell<T, n>::CFDCell(T v)
    : Vector<T, n>(v)
{
  _color = GMlib::Vector<float, 4>(0.0);
  _position = GMlib::Vector<T, 3>(0.0);
  _value = T(0);
  _direction = GMlib::Vector<float, 3>(0.0);
}

template <class T, int n>
GMlib::CFDCell<T, n>::CFDCell(GMlib::Point<T, n> p)
    : Vector<T, n>(p)
{
  _color = GMlib::Vector<float, 4>(0.0);
  _position = GMlib::Vector<T, 3>(0.0);
  _value = T(0);
  _direction = GMlib::Vector<float, 3>(0.0);
}

template <class T, int n>
GMlib::CFDCell<T, n>::CFDCell(GMlib::Point<T, n> p, GMlib::Vector<T, 4> color, GMlib::Vector<T, 3> position, GMlib::Vector<T, 3> direction, T value)
    : Vector<T, n>(p)
{
  _color = color;
  _position = position;
  _value = value;
  _direction = direction;
}

template <class T, int n>
GMlib::CFDCell<T, n>::~CFDCell()
{
}

template <class T, int n>
inline const GMlib::Vector<T, 4> GMlib::CFDCell<T, n>::getColor() const
{
  return _color;
}

template <class T, int n>
inline void GMlib::CFDCell<T, n>::setColor(const GMlib::Vector<T, 4> color)
{
  _color = color;
}

template <class T, int n>
inline void GMlib::CFDCell<T, n>::setColor(const T r, const T g, const T b, const T a)
{
  setColor(GMlib::Vector<T, 4>(r, g, b, a));
}

template <class T, int n>
inline void GMlib::CFDCell<T, n>::setPos(const GMlib::Vector<T, n> pos)
{
  _position = pos;
}

template <class T, int n>
inline void GMlib::CFDCell<T, n>::setValue(const T val)
{
  _value = val;
}

template <class T, int n>
inline void GMlib::CFDCell<T, n>::setDirection(const Vector<T, n> dir)
{
  _direction = dir;
}

template <class T, int n>
inline const GMlib::Vector<T, 3> GMlib::CFDCell<T, n>::getPos() const
{
  return _position;
}

template <class T, int n>
inline const T GMlib::CFDCell<T, n>::getValue() const
{
  return _value;
}

template <class T, int n>
inline const GMlib::Vector<T, 3> GMlib::CFDCell<T, n>::getDirection() const
{
  return _direction;
}

/*! \file gmcfdcell.h
 *
 *  Interface for the PVolume class.
 */

#ifndef __gmCFDCELL_H__
#define __gmCFDCELL_H__

#include "core/types/gmpoint.h"

namespace GMlib{

  template<class T, int n>
  class CFDCell: public Vector<T,n>
    {

  public:
    CFDCell(T v = 0.0);
    CFDCell(Point<T,n> p);
    CFDCell(Point<T,n> p, Vector<T,4> color, Vector<T,3> position, Vector<T,3> direction, T value);


    ~CFDCell();

    const Vector<T, 4>            getColor() const;
    const Vector<T, 3>            getPos() const;
    const T                       getValue() const;
    const Vector<T,3>             getDirection() const;

    void                          setColor(const Vector<T,4> color);
    void                          setColor(const T r, const T g, const T b, const T a);
    void                          setPos(const Vector<T, n> pos);
    void                          setValue(const T val);
    void                          setDirection(const Vector<T, n> dir);



    CFDCell<T,n>& operator=(const CFDCell<T,n>& c)
    {
       //For(int i=0;i<n;i++) _pt[i] = c.pt[i];
       memcpy(this->_pt,c._pt,n*sizeof(T));
      _value = c._value;
      _color = c._color;
      _position = c._position;
      _direction = c._direction;

      return *this;
    }

    CFDCell<T,n> operator*(T d) const
    {
      CFDCell<T,n> c=*this;
      for(int i=0;i<n;i++)
        c._pt[i] = this->_pt[i]*(T)d;

      c._color = _color*T(d);
      c._position = _position*T(d);
      c._direction = _direction*T(d);
      c._value = _value*T(d);

      return c;
    }

    CFDCell<T,n> operator*=(T d) const
    {
      for(int i=0;i<n;i++)
        this->_pt[i]*=(T)d;

      _color *= T(d);
      _position *= T(d);
      _direction *= _direction*T(d);
      _value *= _value*T(d);

      return *this;
    }

    CFDCell<T,n>& operator+=(const CFDCell<T,n>& c)
      {
      for(int i=0;i<n;i++)
        this->_pt[i]+=c._pt[i];

      _color += c._color;
      _position += c._position;
      _direction += c._direction;
      _value += c._value;

      return *this;
    }

    CFDCell<T,n>& operator*=(const CFDCell<T,n>& c)
      {
      for(int i=0;i<n;i++)
        this->_pt[i]*=c._pt[i];

      _color*=c._color;
      _position*=c._position;
      _direction *= c._direction;
      _value *= c._value;

      return *this;
    }

  private:
    Vector<T,4>                   _color;
    Vector<T,3>                   _position;
    T                             _value;
    Vector<T,3>                   _direction;


    };

  } //END G

#include "gmcfdcell.c"

#endif // __gmCFDCELL_H__


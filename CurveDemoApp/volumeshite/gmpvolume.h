/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/



/*! \file gmpvolume.h
 *
 *  Interface for the PVolume class.
 */

#ifndef __gmDVOLUME_H__
#define __gmDVOLUME_H__


#include "gmpvolumevisualizer.h"
#include "gmcfdcell.h"
// gmlib
#include <parametrics/gmparametrics>
#include <core/containers/gmarray.h>
#include <core/containers/gmdvector.h>
#include <core/containers/gmdmatrix.h>
#include <core/containers/gmdvectorn.h>
//#include <core/containers/gmcfdcell.h>
//#include <parametrics/visualizers/gmpvolumevisualizer.h>


// stl
//#include <fstream>


namespace GMlib {

  template<typename T, int n>
  class Selector;

  template<typename T>
  class SelectorGridVisualizer;

  template <typename T,int n>
  class PVolume : public Parametrics<T,3,n> {

  public:

    PVolume(int s1 = 20, int s2 = 20, int s3 = 20);
    PVolume(const PVolume<T,n>& copy);
    virtual ~PVolume();


    T                             getParDeltaU();    //Get the size in u direction
    T                             getParDeltaV();    //Get the size in v direction
    T                             getParDeltaW();    //Get the size in w direction
    T                             getParStartU();    //Get the translated start position in u
    T                             getParStartV();    //Get the translated start position in v
    T                             getParStartW();    //Get the translated start position in w
    T                             getParEndU();    //Get the translated end positon in u
    T                             getParEndV();    //Get the translated end positon in v
    T                             getParEndW();    //Get the translated end positon in w
    CFDCell<T,n>&                  getPosition( T u, T v , T w);    //eval first then get position
    int                           getSamPU( int i = 0 ) const;    //return number of samples for the partition given in u
    int                           getSamPV( int i = 0 ) const;    //return number of samples for the partition given in v
    int                           getSamPW( int i = 0 ) const;    //return number of samples for the partition given in w
    int                           getSamplesU() const;    //return number of samples u for single sampling
    int                           getSamplesV() const;    //return number of samples v for single sampling
    int                           getSamplesW() const;    //return number of samples w for single sampling
    virtual bool                  isClosedU() const;
    virtual bool                  isClosedV() const;
    virtual bool                  isClosedW() const;

    void                          setDomainU( T start, T end );
    void                          setDomainUScale( T sc );
    void                          setDomainUTrans( T tr );
    void                          setDomainV( T start, T end );
    void                          setDomainVScale( T sc );
    void                          setDomainVTrans( T tr );
    void                          setDomainW( T start, T end );
    void                          setDomainWScale( T sc );
    void                          setDomainWTrans( T tr );
    void                          setNoDer( int d );

    virtual void                  setSurroundingSphere(const DVectorN < DVectorN< CFDCell<T,n>, 3, T>,3 >& p);
    void                          enableDefaultVisualizer(bool enable = true);
    const PVolumeVisualizer<T,n>* getDefaultVisualizer() const;
    void                          toggleDefaultVisualizer();
    virtual void                  toggleSelectorGridVisualizer();


    bool                          edited;

    void                          insertVisualizer( Visualizer *visualizer);
    void                          removeVisualizer( Visualizer *visualizer);

    T                             shiftU(T u);    // Get the scaled position in u-direction used for last evaluation
    T                             shiftV(T v);    // Get the scaled position in v-direction used for last evaluation
    T                             shiftW(T W);    // Get the scaled position in w-direction used for last evaluation

    CFDCell<T,n>&                 getDerU(T u, T v, T w);
    CFDCell<T,n>&                 getDerUU(T u, T v, T w);
    CFDCell<T,n>&                 getDerV(T u, T v, T w);
    CFDCell<T,n>&                 getDerVV(T u, T v, T w);
    CFDCell<T,n>&                 getDerW(T u, T v, T w);
    CFDCell<T,n>&                 getDerWW(T u, T v, T w);
    CFDCell<T,n>&                 getDerUV(T u, T v, T w);
    CFDCell<T,n>&                 getDerUW(T u, T v, T w);
    CFDCell<T,n>&                 getDerVW(T u, T v, T w);


    DVectorN<CFDCell<T,n>, 3, T>&    evaluate( const Point<T,3>& p, const Point<T,3>& d);
    DVectorN<CFDCell<T,n>, 3, T>&    evaluate( T u, T v, T w, int d1, int d2, int d3);


    const Point<T,n>&             operator () ( T u, T v , T w); //evaluate first then retrun postion

    virtual void                  preSample( int m1, int m2, int m3, int d1, int d2, int d3, T s_u = T(0), T s_v = T(0), T s_w = T(0),T e_u = T(0), T e_v = T(0), T e_w = T(0) );
    void                          resample(DVectorN< DVectorN < DVectorN< CFDCell<T,n>, 3, T>, 3, T >, 3, T>& p, int m1, int m2,int m3, int d1, int d2, int d3);
    virtual void                  resample(DVectorN < DVectorN< CFDCell<T,n>, 3, T>, 3, T >& p, int m1, int m2, int m3,int d1, int d2, int d3, T s_u = T(0), T s_v = T(0), T s_w = T(0), T e_u = T(0), T e_v = T(0), T e_w = T(0));
    virtual void                  replot( int m1 = 0, int m2 = 0, int m3 = 0,int d1 = 0, int d2 = 0, int d3 = 0 );


   void                           updateVisualizers();

  protected:

    Array<PVolumeVisualizer<T,n>* >             _pvolume_visualizers;

    PVolumeVisualizer<T,n>          *_default_visualizer;
    SelectorGridVisualizer<T>       *_selector_grid_visualizer;
    DVectorN< Selector<T,3>*,3,T>   _selectors;

    int                           _no_sam_u;    // Number of samples u for single sampling
    int                           _no_sam_v;    // Number of samples v for single sampling
    int                           _no_sam_w;    // Number of samples q for single sampling

    int                           _no_der_u;    // Number of derivatives u
    int                           _no_der_v;    // Number of derivatives v
    int                           _no_der_w;    // Number of derivatives w

    DVector< Vector<T,2> >        _sam_p_u;     // Sample partition u (start/stop)
    DVector< Vector<T,2> >        _sam_p_v;     // Sample partition v (start/stop)
    DVector< Vector<T,2> >        _sam_p_w;     // Sample partition w (start/stop)

    DVector< int >                _no_sam_p_u;  // Number of samples for each sampling partition in u
    DVector< int >                _no_sam_p_v;  // Number of samples for each sampling partition in v
    DVector< int >                _no_sam_p_w;  // Number of samples for each sampling partition in w

    // Used by operator() for number of derivative to evaluate.
    int                           _default_d;


    // The result of the previous evaluation
    DVectorN< CFDCell<T,n>, 3, T> _p;           // Position and belonging partial derivatives
    DVectorN<DVectorN< CFDCell<T,n>, 3, T>,3, T> _pp;           // Position and belonging partial derivatives
    T                             _u;           // The parameter value in u-direction used for last evaluation
    T                             _v;           // The parameter value in v-direction used for last evaluation
    T                             _w;           // The parameter value in w-direction used for last evaluation
    int                           _d1;          // Number of derivatives in u-direction computed last time
    int                           _d2;          // Number of derivatives in v-direction computed last time
    int                           _d3;          // Number of derivatives in w-direction computed last time

    // Shift of parameter
    T                             _tr_u;        // Translate u-parametre
    T                             _sc_u;        // Scale u-parametre

    T                             _tr_v;        // Translate v-parametre
    T                             _sc_v;        // Scale v-parametre

    T                             _tr_w;        // Translate w-parametre
    T                             _sc_w;        // Scale w-parametre


    void                          findIndex(T u, T v, T w, int& i, int& j, int& k);

    /*! virtual void PSurf<T,3>::eval( T u, T v, int d1, int d2, bool lu , bool lv ) = 0
     *  Surface evaluator. (Requires implementation in PSurf sub-classes.)
     *  \param[in]  u   Evaluation parameter in u-direction.
     *  \param[in]  v   Evaluation parameter in v-direction.
     *  \param[in]  w   Evaluation parameter in w-direction.
     *  \param[in]  d1  Number of derivatives to be computed for u.
     *  \param[in]  d2  Number of derivatives to be computed for v.
     *  \param[in]  d3  Number of derivatives to be computed for w.
     *  \param[in]  lu  Whether to evaluate from left in u.
     *  \param[in]  lv  Whether to evaluate from left in v.
     ** \param[in]  lv  Whether to evaluate from left in w.
     */
    virtual void                  eval( T u, T v, T w, int d1, int d2, int d3, bool lu = true, bool lv = true, bool lw = true ) = 0;

    /*! virtual T PSurf<T,3>::getEndPU() = 0
     *  Returns the parametric end value in u-direction. (Requires implementation in PVolume sub-classes.)
     *  \return Parametric end value in u.
     */
    virtual T                     getEndPU() = 0;

    /*! virtual T PSurf<T,3>::getEndPV() = 0
     *  Returns the parametric end value in v-direction. (Requires implementation in PVolume sub-classes.)
     *  \return Parametric end value in v.
     */
    virtual T                     getEndPV() = 0;

    /*! virtual T PSurf<T,3>::getEndPW() = 0
     *  Returns the parametric end value in w-direction. (Requires implementation in PVolume sub-classes.)
     *  \return Parametric end value in w.
     */
    virtual T                     getEndPW() = 0;

    /*! virtual T PSurf<T,3>::getStartPU() = 0
     *  Returns the parametric start value in u-direction. (Requires implementation in PVolume sub-classes.)
     *  \return Parametric start value in u.
     */
    virtual T                     getStartPU() = 0;

    /*! virtual T PSurf<T,3>::getStartPV() = 0
     *  Returns the parametric start value in v-direction. (Requires implementation in PVolume sub-classes.)
     *  \return Parametric start value in v.
     */
    virtual T                     getStartPV() = 0;

    /*! virtual T PSurf<T,3>::getStartPW() = 0
     *  Returns the parametric start value in w-direction. (Requires implementation in PVolume sub-classes.)
     *  \return Parametric start value in w.
     */
    virtual T                     getStartPW() = 0;

  private:
    void                          _eval(T u, T v, T w, int d1, int d2, int d3);

    }; // END class PVolume

  } // END namespace GMlib

// Include PVolume class function implementations
#include "gmpvolume.c"


#endif  //__gmDVOLUME_H__

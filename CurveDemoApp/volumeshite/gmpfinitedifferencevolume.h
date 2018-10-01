/*! \file gmpfinitedifferencevolume.h
 *
 *  Interface for the PFiniteDifferenceVolume class.
 */

#ifndef __gmDFINITEDIFFERENCEVOLUME_H__
#define __gmDFINITEDIFFERENCEVOLUME_H__

#include "gmpvolume.h"

namespace GMlib {

template <typename T, int n, int m=n>
class PFiniteDifferenceVolume : public PVolume<T, n> {
    GM_SCENEOBJECT(PFiniteDifferenceVolume)
    public:
        PFiniteDifferenceVolume();
    PFiniteDifferenceVolume(Vector<int,3> dim, int order);
    PFiniteDifferenceVolume(DVectorN<CFDCell<T, n>, 3, T> c, int order);
    ~PFiniteDifferenceVolume();


    void                                          setPointSet(DVectorN<CFDCell<T, n>, 3, T> c, int order);
    void                                          replot(int m1, int m2, int m3, int d1, int d2, int d3);

    DVectorN<DVectorN<CFDCell<T, n>, 3, T>, n>    setFromF(){return _fSet;}

protected:
    //Inherited from PVolume
    void                                          eval( T u, T v, T w, int d1, int d2, int d3, bool lu = true, bool lv = true, bool lw = true );
    T                                             getEndPU();
    T                                             getEndPV();
    T                                             getEndPW();
    T                                             getStartPU();
    T                                             getStartPV();
    T                                             getStartPW();


    void                                          forwardDiffU(int i,int j,int k,int d1,int d2,int d3);              //Numerical forward difference scheme in u direction.
    void                                          backwardDiffU(int i,int j,int k,int d1,int d2,int d3);             //Numerical backward difference scheme in u direction.
    void                                          symetricDiffU(int i,int j,int k,int d1,int d2,int d3);             //Numerical symetric difference scheme in u direction.
    void                                          forwardDiffV(int i,int j,int k,int d1,int d2,int d3);              //Numerical forward difference scheme in v direction.
    void                                          backwardDiffV(int i,int j,int k,int d1,int d2,int d3);             //Numerical backward difference scheme in v direction.
    void                                          symetricDiffV(int i,int j,int k,int d1,int d2,int d3);             //Numerical symetric difference scheme in v direction.
    void                                          forwardDiffW(int i,int j,int k,int d1,int d2,int d3);              //Numerical forward difference scheme in w direction.
    void                                          backwardDiffW(int i,int j,int k,int d1,int d2,int d3);             //Numerical backward difference scheme in w direction.
    void                                          symetricDiffW(int i,int j,int k,int d1,int d2,int d3);             //Numerical symetric difference scheme in w direction.


private:
    void                                          init();
    int                                           _xdif;
    int                                           _ydif;
    int                                           _zdif;
    DVectorN<CFDCell<T, n>, 3, T>           _pSet;// The point set.
    DVectorN<DVectorN<CFDCell<T, n>, 3, T>, n>   _fSet; // The full set of points with derivatives.
    int                                           _order; // Number of derivatives to calculate.

}; // END class PFiniteDifferenceVolume

} // END namespace GMlib

// Include PFiniteDifferenceVolume class function implementations
#include "gmpfinitedifferencevolume.c"


#endif // __gmDFINITEDIFFERENCEVOLUME_H__

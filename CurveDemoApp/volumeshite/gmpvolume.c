

/*! \file gmpvolume.c
 *
 *  Implementation of the PVolume template class.
 */

//
#include "gmpvolume.h"
#include "gmpvolumedefaultvisualizer.h"
#include <scene/selector/gmselector.h>

namespace GMlib{

template<typename T, int n>
PVolume<T, n>::PVolume(int s1, int s2, int s3)
{
    _no_sam_u                        = s1;
    _no_sam_v                        = s2;
    _no_sam_w                        = s3;
    _no_der_u                        = 1;
    _no_der_v                        = 1;
    _no_der_w                        = 1;

    _tr_u               = T(0);
    _tr_v               = T(0);
    _tr_w               = T(0);
    _sc_u               = T(1);
    _sc_v               = T(1);
    _sc_w               = T(1);

    _default_visualizer = 0x0;
    _selector_grid_visualizer = 0x0;
    //    this->_has_depth = false;

}

template<typename T, int n>
PVolume<T, n>::PVolume(const PVolume<T, n> &copy)
{
    _p            = copy._p;
    _default_visualizer = 0x0;
    //    this->_has_depth = false;

}

template<typename T, int n>
PVolume<T, n>::~PVolume()
{
    enableDefaultVisualizer( false );
    if( _default_visualizer )
        delete _default_visualizer;
}


template <typename T, int n>
inline
T PVolume<T, n>::getParDeltaU()
{
    return _sc_u * (getEndPU() - getStartPU());
}


template <typename T, int n>
inline
T PVolume<T, n>::getParDeltaV()
{
    return _sc_v * (getEndPV() - getStartPV());
}


template <typename T, int n>
inline
T PVolume<T, n>::getParDeltaW()
{
    return _sc_w * (getEndPW() - getStartPW());
}


template <typename T, int n>
inline
T PVolume<T, n>::getParStartU()
{
    return getStartPU() + _tr_u;
}


template <typename T, int n>
inline
T PVolume<T, n>::getParStartV()
{
    return getStartPV() + _tr_v;
}


template <typename T, int n>
inline
T PVolume<T, n>::getParStartW()
{
    return getStartPW() + _tr_w;
}


template <typename T, int n>
inline
T PVolume<T, n>::getParEndU()
{
    return getParStartU() + getParDeltaU();
}


template <typename T, int n>
inline
T PVolume<T, n>::getParEndV()
{
    return getParStartV() + getParDeltaV();
}


template <typename T, int n>
inline
T PVolume<T, n>::getParEndW()
{
    return getParStartV() + getParDeltaW();
}

template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getPosition(T u, T v, T w)
{
    _eval(u, v, w, 0, 0, 0);
    return _p[0][0][0];
}

template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getDerU(T u, T v, T w)
{
    _eval(u, v, w, 1, 0, 0);
    return this->_present*_p[1][0][0];
}


template <typename T, int n>
inline
int PVolume<T, n>::getSamPU(int i) const
{
    return _no_sam_p_u(i);
}


template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getDerUU(T u, T v, T w)
{
    _eval(u, v, w, 2, 0, 0);
    return this->_present*_p[2][0][0];
}


template <typename T, int n>
inline
int PVolume<T, n>::getSamPV(int i) const
{
    return _no_sam_p_v(i);
}


template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getDerV(T u, T v, T w)
{
    _eval(u, v, w, 0, 1, 0);
    return this->_present*_p[0][1][0];
}


template <typename T, int n>
inline
int PVolume<T, n>::getSamPW(int i) const
{
    return _no_sam_p_w(i);
}


template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getDerVV(T u, T v, T w)
{
    _eval(u, v, w, 0, 2, 0);
    return this->_present*_p[0][2][0];
}


template <typename T, int n>
inline
int PVolume<T, n>::getSamplesU() const
{
    return _no_sam_u;
}


template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getDerW(T u, T v, T w)
{
    _eval(u, v, w, 0, 0, 1);
    return this->_present*_p[0][0][1];
}


template <typename T, int n>
inline
int PVolume<T, n>::getSamplesV() const
{
    return _no_sam_v;
}


template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getDerWW(T u, T v, T w)
{
    _eval(u, v, w, 0, 0, 2);
    return this->_present*_p[0][0][2];
}


template <typename T, int n>
inline
int PVolume<T, n>::getSamplesW() const
{
    return _no_der_w;
}

template <typename T, int n>
inline
bool PVolume<T,n>::isClosedU() const
{
    return false;
}

template <typename T, int n>
inline
bool PVolume<T,n>::isClosedV() const
{
    return false;
}


template <typename T, int n>
inline
bool PVolume<T,n>::isClosedW() const
{
    return false;
}



template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getDerUV(T u, T v, T w)
{
    _eval(u, v, w, 2, 2, 0);
    return this->_present*_p[2][2][0];
}


template <typename T, int n>
inline
void PVolume<T, n>::setDomainU(T start, T end)
{
    _sc_u = (end -start) / (getEndPU() - getStartPU());
    _tr_u = start - getStartPU();
}


template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getDerUW(T u, T v, T w)
{
    _eval(u, v, w, 2, 0, 2);
    return this->_present*_p[2][0][2];
}


template <typename T, int n>
inline
void PVolume<T, n>::setDomainUScale(T sc)
{
    _sc_u = sc;
}


template <typename T, int n>
inline
CFDCell<T,n> &PVolume<T, n>::getDerVW(T u, T v, T w)
{
    _eval(u, v, w, 0, 2, 2);
    return this->_present*_p[0][2][2];
}


template <typename T, int n>
inline
void PVolume<T, n>::setDomainUTrans(T tr)
{
    _tr_u = tr;
}


template <typename T, int n>
inline
void PVolume<T, n>::setDomainV(T start, T end)
{
    _sc_v = (end -start) / (getEndPV() - getStartPV());
    _tr_v = start - getStartPV();
}


template <typename T, int n>
inline
void PVolume<T, n>::setDomainVScale(T sc)
{
    _sc_v = sc;
}


template <typename T, int n>
inline
DVectorN<CFDCell<T,n>, 3, T> &PVolume<T, n>::evaluate(const Point<T, 3> &p, const Point<T, 3> &d)
{
    return evaluate( p(0), p(1), p(2), d(0), d(1), d(2));
}


template <typename T, int n>
inline
void PVolume<T, n>::setDomainVTrans(T tr)
{
    _tr_v = tr;
}


template <typename T, int n>
inline
void PVolume<T, n>::setDomainW(T start, T end)
{
    _sc_w = (end -start) / (getEndPW() - getStartPW());
    _tr_w = start - getStartPW();
}


template <typename T, int n>
inline
void PVolume<T, n>::setDomainWScale(T sc)
{
    _sc_w = sc;
}


template <typename T, int n>
inline
DVectorN<CFDCell<T,n>, 3, T> &PVolume<T, n>::evaluate(T u, T v, T w, int d1, int d2, int d3)
{
    _eval( u, v, w, d1, d2, d3);
    return _p;
}


template <typename T, int n>
inline
void PVolume<T, n>::setDomainWTrans(T tr)
{
    _tr_w = tr;
}


template <typename T, int n>
inline
void PVolume<T, n>::setNoDer(int d)
{
    _default_d  = d;
}


template <typename T, int n>
inline
void PVolume<T, n>::setSurroundingSphere(const DVectorN<DVectorN<CFDCell<T, n>, 3, T>, 3> &p)
{
    Sphere<T,n> s( p(0)(0)(0)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]-1)(p.getDim()[1]-1)(p.getDim()[2]-1)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]/2)(p.getDim()[1]/2)(p.getDim()[2]/2)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]-1)(p.getDim()[1]-1)(0)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]-1)(p.getDim()[1]-1)(p.getDim()[2]/2)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]-1)(p.getDim()[1]/2)(p.getDim()[2]/2)(0)(0)(0)(0));
    s += Point<T,n>( p(0)(p.getDim()[1]-1)(p.getDim()[2]-1)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]/2)(p.getDim()[1]-1)(p.getDim()[2]-1)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]/2)(p.getDim()[1]-1)(p.getDim()[2]/2)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]-1)(0)(p.getDim()[2]-1)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]-1)(p.getDim()[1]/2)(p.getDim()[2]-1)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]/2)(p.getDim()[1]/2)(p.getDim()[2]-1)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]-1)(0)(0)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]/2)(0)(0)(0)(0)(0)(0));
    s += Point<T,n>( p(0)(p.getDim()[1]-1)(0)(0)(0)(0)(0));
    s += Point<T,n>( p(0)(p.getDim()[1]/2)(0)(0)(0)(0)(0));
    s += Point<T,n>( p(0)(0)(p.getDim()[2]-1)(0)(0)(0)(0));
    s += Point<T,n>( p(0)(0)(p.getDim()[2]/2)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]/2)(p.getDim()[1]/2)(0)(0)(0)(0)(0));
    s += Point<T,n>( p(0)(p.getDim()[1]/2)(p.getDim()[2]/2)(0)(0)(0)(0));
    s += Point<T,n>( p(0)(p.getDim()[1]/2)(p.getDim()[2]-1)(0)(0)(0)(0));
    s += Point<T,n>( p(0)(p.getDim()[1]-1)(p.getDim()[2]/2)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]/2)(0)(p.getDim()[2]-1)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]/2)(p.getDim()[1]-1)(0)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]-1)(0)(p.getDim()[2]/2)(0)(0)(0)(0));
    s += Point<T,n>( p(p.getDim()[0]-1)(p.getDim()[1]/2)(0)(0)(0)(0)(0));

    Parametrics<T,3,n>::setSurroundingSphere(s);
}

template <typename T, int n>
inline
void PVolume<T, n>::enableDefaultVisualizer(bool enable)
{
    if( !enable)
    {
        removeVisualizer(_default_visualizer);
        _pvolume_visualizers.remove(_default_visualizer);
    }
    else {
        if( !_default_visualizer )
            _default_visualizer = new PVolumeDefaultVisualizer<T,n>();

        _pvolume_visualizers.insert(_default_visualizer);
        insertVisualizer( _default_visualizer );
    }
}

template <typename T, int n>
inline
const PVolumeVisualizer<T, n> *PVolume<T, n>::getDefaultVisualizer() const
{
    return _default_visualizer;
}

template <typename T, int n>
inline
void PVolume<T, n>::toggleDefaultVisualizer()
{
    if( !_pvolume_visualizers.exist(_default_visualizer) )
        enableDefaultVisualizer( true );
    else
        enableDefaultVisualizer( false );
}

template <typename T, int n>
inline
void PVolume<T,n>::toggleSelectorGridVisualizer()
{

}

template <typename T, int n>
inline
void PVolume<T, n>::insertVisualizer(Visualizer *visualizer)
{
    if(!visualizer)
        return;

    //    DisplayObject::insertVisualizer( visualizer );
    SceneObject::insertVisualizer(visualizer);

    PVolumeVisualizer<T,n> *visu = dynamic_cast<PVolumeVisualizer<T,n>*>( visualizer );

    PVolumeDefaultVisualizer<T,n> *defVisu = dynamic_cast<PVolumeDefaultVisualizer<T,n>*>( visualizer );
    //    SelectorGridVisualizer<T> *sgVisu = dynamic_cast<SelectorGridVisualizer<T>*>( visualizer );

    if( defVisu )
    {
        _default_visualizer = defVisu;
    }

    //    if(sgVisu)
    //    {
    //        _selector_grid_visualizer = sgVisu;
    //    }

    if(!visu)
        return;

    _pvolume_visualizers.insert(visu);
}


template <typename T, int n>
inline
void PVolume<T, n>::removeVisualizer(Visualizer *visualizer)
{
    SceneObject::removeVisualizer( visualizer );
}


template <typename T, int n>
inline
void PVolume<T, n>::_eval(T u, T v, T w, int d1, int d2, int d3)
{
    if( !(d1 <= _d1 && d2 <=_d2 && d3 <= _d3 && u == _u && v == _v && w == _w) ) {
        _u = u;
        _v = v;
        _w = w;
        _d1 = d1;
        _d2 = d2;
        _d3 = d3;

        eval( shiftU(u), shiftV(v), shiftW(w), d1, d2, d3 );
    }
}


template <typename T, int n>
inline
T PVolume<T, n>::shiftU(T u)
{
    return getStartPU() + _sc_u * (u -getParStartU());
}


template <typename T, int n>
inline
T PVolume<T, n>::shiftV(T v)
{
    return getStartPV() + _sc_v * (v -getParStartV());
}



template <typename T, int n>
inline
T PVolume<T, n>::shiftW(T w)
{
    return getStartPW() + _sc_w * (w -getParStartW());
}


template <typename T, int n>
inline
const Point< T, n> &PVolume< T , n>::operator ()(T u, T v, T w){

    _eval(u, v, w, _default_d, _default_d,_default_d);
    return static_cast<Point<T,n> >(_p[0][0]);
}


template <typename T, int n>
inline
void PVolume<T, n>::preSample(
        int /*m1*/, int /*m2*/, int /*m3*/, int /*d1*/, int /*d2*/, int /*d3*/,
        T /*s_u*/, T /*s_v*/, T /*s_w*/, T /*e_u*/, T /*e_v*/, T /*e_w*/
        ) {}


/**
   *
   *s_u = start u
   *e_u = end u
   *Makes a matrix with positions and belonging partial derivatives, based on evaluation of every point
   *Handles boundaries
   */

template <typename T, int n>
void PVolume<T, n>::resample(DVectorN < DVectorN< CFDCell<T,n>, 3, T>, 3, T >& p, int m1, int m2, int m3,int d1, int d2, int d3, T s_u, T s_v, T s_w, T e_u, T e_v, T e_w) {

    T du = (e_u-s_u)/(m1-1); //delta u
    T dv = (e_v-s_v)/(m2-1); //delta v
    T dw = (e_w-s_w)/(m3-1); //delta w

    p.setDim(Vector<int,3>(m1, m2, m3));

    for(int i=0; i<m1-1; i++) {

        T u = s_u + i*du;
        for(int j=0;j<m2-1;j++) {

            T v = s_v + j*dv;

            for(int k = 0; k <m3-1; k++){
                eval(u, v, s_w + k*dw, d1, d2,d3, true, true, true );
                p[i][j][k] = _p;
            }

            eval(u, v, e_w, d1, d2,d3,true, true, false);
            p[i][j][m3-1] = _p;
        }

        for(int k = 0; k <m3-1; k++){
            eval(u, e_v, s_w + k*dw, d1, d2,d3, true, false, true );
            p[i][m2-1][k] = _p;
        }

        eval(u, e_v, e_w, d1, d2,d3,true, false, false);
        p[i][m2-1][m3-1] = _p;
    }

    for(int j=0;j<m2-1;j++) {
        T v = s_v + j*dv;
        for(int k = 0; k <m3-1; k++){
            eval(e_u, v, s_w + k*dw, d1, d2,d3, false, true, true );
            p[m1-1][j][k] = _p;
        }

        eval(e_u, s_v + j*dv, e_w, d1, d2,d3, false, true, false);
        p[m1-1][j][m3-1] = _p;
    }

    for(int k = 0; k <m3-1; k++){
        eval(e_u, e_v, s_w + k*dw, d1, d2,d3, false, false, true );
        p[m1-1][m2-1][k] = _p;
    }

    eval(e_u, e_v,e_w, d1, d2,d3, false, false,false);
    p[m1-1][m2-1][m3-1] = _p;

    //    switch( this->_dm ) {
    //    case GM_DERIVATION_EXPLICIT:
    //      // Do nothing, evaluator algorithms for explicite calculation of derivatives
    //      // should be defined in the eval( ... ) function enclosed by
    //      // if( this->_derivation_method == this->EXPLICIT ) { ... eval algorithms for derivatives ... }
    //      break;

    //      //    case GM_DERIVATION_DD:
    //      //    default:
    //      //      _evalDerDD( p, d1, d2, du, dv );
    //      //      break;
    //    }

}


template <typename T, int n>
inline
void PVolume<T, n>::resample( DVectorN< DVectorN < DVectorN< CFDCell<T,n>, 3, T>, 3, T >, 3, T>& p,
                              int m1, int m2,int m3, int d1, int d2, int d3) {

    resample( p, m1, m2,m3, d1, d2, d3,
              getStartPU(), getStartPV(),getStartPW(),
              getEndPU(), getEndPV(), getEndPW() );
}


template <typename T, int n>
void PVolume<T, n>::replot( int m1, int m2,int m3, int d1, int d2, int d3 ) {

    //    // Correct sample domain
    if( m1 < 2 )
        m1 = _no_sam_u;
    else
        _no_sam_u = m1;

    if( m2 < 2 )
        m2 = _no_sam_v;
    else
        _no_sam_v = m2;

    if( m3 < 2 )
        m3 = _no_sam_w;
    else
        _no_sam_w = m3;

    // Correct derivatives
    if( d1 < 1 )
        d1 = _no_der_u;
    else
        _no_der_u = d1;

    if( d2 < 1 )
        d2 = _no_der_v;
    else
        _no_der_v = d2;

    if( d3 < 1 )
        d3 = _no_der_w;
    else
        _no_der_w = d3;


    // pre-sampel / pre evaluate data for a given parametric surface, if wanted/needed
    preSample(
                m1, m2,m3, d1, d2,d3,
                getStartPU(),
                getStartPV(),
                getStartPW(),
                getEndPU(),
                getEndPV(),
                getEndPW()
                );


    // Sample Positions and related Derivatives
    DVectorN < DVectorN< CFDCell<T,n>, 3, T>, 3, T > p;
    resample(
                p, m1, m2,m3, d1, d2,d3,
                getStartPU(),
                getStartPV(),
                getStartPW(),
                getEndPU(),
                getEndPV(),
                getEndPW()
                );

    //    // Sample Normals
    //    DMatrix< Vector<T,n> > normals;
    //    resampleNormals( p, normals );

    // Set The Surrounding Sphere
    setSurroundingSphere( p );

//    std::cout << "Runs?" << std::endl;
    _pp = p;

    // Replot Visaulizers
    //    if(edited)
    for( int i = 0; i < this->_pvolume_visualizers.getSize(); i++ )
        this->_pvolume_visualizers[i]->replot( p, m1, m2,m3, d1, d2, d3, isClosedU(), isClosedV(), isClosedW() );
}

template<typename T, int n>
void PVolume<T,n>::updateVisualizers()
{
    //    std::cout << "Updating visualizers..." << std::endl;
    if(_default_visualizer)
        this->_default_visualizer->replot( _pp, 10, 10, 10, 0, 0, 0, false, false, false);
}

template <typename T, int n>
inline
void PVolume<T, n>::findIndex(T u, T v, T w, int &i, int &j, int &k)
{
    //    T t = getParStartU();
    //    T t2 = getParDeltaU();
    //    T t3 = getParEndU();
    i = ( _no_sam_u-1)*( u-getParStartU())/(getParDeltaU())+0.1;
    j = ( _no_sam_v-1)*( v-getParStartV())/(getParDeltaV())+0.1;
    k = ( _no_sam_w-1)*( w-getParStartW())/(getParDeltaW())+0.1;
}

}


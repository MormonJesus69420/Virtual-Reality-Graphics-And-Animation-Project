#ifndef GMPVOLUMEVISUALIZER_H
#define GMPVOLUMEVISUALIZER_H

#include "gmcfdcell.h"

// gmlib
#include <core/containers/gmarray.h>
#include <core/containers/gmdvectorn.h>
#include <core/types/gmpoint.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/gmtexture.h>
#include <scene/gmvisualizer.h>

namespace GMlib {

template <typename T, int n>
class PVolumeVisualizer : public Visualizer {
  public:
  PVolumeVisualizer();
  PVolumeVisualizer(const PVolumeVisualizer<T, n>& copy);

  virtual void replot(const DVectorN<DVectorN<CFDCell<T, n>, 3, T>, 3, T>& p,
      int m1, int m2, int m3, int d1, int d2, int d3,
      bool closed_u, bool closed_v, bool closed_w);

  static void fillCubeVBO(GL::VertexBufferObject& vbo, T size);
  static void fillCubeIBO(GL::IndexBufferObject& ibo, GLuint& no_indices);

  static void fill3DTexture(const DVectorN<Vector<GLfloat, 4>, n>& color, int m1, int m2, int m3, bool closed_u, bool closed_v, bool closed_w, GL::Texture& tex);
  static void fillTransferFunction(GL::Texture& transfertex, const std::vector<Vector<float, 4>>& rgba, bool update = false);

  //    static void   fillTriangleStripIBO(GL::IndexBufferObject& ibo, int m1, int m2, GLuint& no_strips, GLuint& no_strip_indices, GLsizei& strip_size );
  //    static void   fillNMap( GL::Texture& nmap, const DMatrix< Vector<T, 3> >& normals, bool closed_u, bool closed_v);
  //    static void   compTriangleStripProperties( int m1, int m2, GLuint& no_strips, GLuint& no_strip_indices, GLsizei& strip_size );

  //    static void   fillMap( GL::Texture& map, const DMatrix< DMatrix< Vector<T,n> > >& p, int d1, int d2, bool closed_u, bool closed_v );
  //    static void   fillStandardIBO( GLuint vbo_id, int m1, int m2 );
  //    static void   fillTriangleStripTexVBO( GLuint vbo_id, int m1, int m2 );
  //    static void   fillTriangleStripNormalVBO( GLuint vbo_id, DMatrix< Vector<T,3> >& normals );
  //    static void   fillTriangleStripVBO( GLuint vbo_id, DMatrix< DMatrix< Vector<T,n> > >& p, int d1 = 0, int d2 = 0 );
  //    static void   getTriangleStripDataInfo( const DMatrix< DMatrix< Vector<T,n> > >& p, int& no_dp, int& no_strips, int& no_verts_per_strips );

}; // END class PSurfVisualizer

} // END namespace GMlib

// Include PSurfVisualizer class function implementations
#include "gmpvolumevisualizer.c"

#endif // GMPVOLUMEVISUALIZER_H

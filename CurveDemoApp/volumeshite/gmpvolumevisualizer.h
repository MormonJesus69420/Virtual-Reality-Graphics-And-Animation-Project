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

}; // END class PSurfVisualizer

} // END namespace GMlib

// Include PSurfVisualizer class function implementations
#include "gmpvolumevisualizer.c"

#endif // GMPVOLUMEVISUALIZER_H

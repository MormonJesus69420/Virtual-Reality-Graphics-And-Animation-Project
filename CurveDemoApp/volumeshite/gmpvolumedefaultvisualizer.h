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

#ifndef GMPVOLUMEDEFAULTVISUALIZER_H
#define GMPVOLUMEDEFAULTVISUALIZER_H

#include "gmpvolumevisualizer.h"
#include "transferfunction.h"

// gmlib
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <opengl/bufferobjects/gmuniformbufferobject.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/gmtexture.h>
#include <scene/gmdefaultrenderer>
#include <scene/gmsceneobject>

namespace GMlib {

enum VolumeDisplayMode { DM_COLOR,
  DM_COLOR_DERU };

template <typename T, int n>
class PVolumeDefaultVisualizer : public PVolumeVisualizer<T, n> {
  GM_VISUALIZER(PVolumeDefaultVisualizer)
  public:
  PVolumeDefaultVisualizer();
  //    PVolumeDefaultVisualizer( const PVolumeDefaultVisualizer<T,n>& copy );

  void render(const GMlib::SceneObject* obj, const GMlib::DefaultRenderer* renderer) const;
  void renderGeometry(const GMlib::SceneObject* obj, const GMlib::Renderer* renderer, const GMlib::Color& color) const;

  virtual void replot(const DVectorN<DVectorN<CFDCell<T, n>, 3, T>, 3, T>& p,
      int m1, int m2, int m3, int d1, int d2, int d3,
      bool closed_u, bool closed_v, bool closed_w);

  void setTransferValues(const std::vector<Vector<float, 4>>& rgba, bool update = false);
  void setTestTexture(unsigned char* volume_);
  void setVolumeDisplayMode(VolumeDisplayMode mode);
  void setShaders(bool colorMip, bool alphaMip, bool blackWhite, bool depthTerminate, bool depthStart, bool slicing);
  void setSlicingVector(Vector<float, 3> slicingVector);
  void setSlicingVector(float x, float y, float z);
  void setTransferType(int index);
  void toggleTransferValues();
  void updateTransferValues(bool update);
  TransferFunction* getTransferFunction() { return _tf; }

  private:
  GL::VertexBufferObject _vbo;
  GL::VertexBufferObject _tbo;
  GL::IndexBufferObject _ibo;
  GL::BufferObject _bo_cube;
  GL::BufferObject _bo_cube_indices;
  GL::BufferObject _bo_cube_frame_indices;
  GL::Texture _depth_tex;
  GL::Texture _transfer_tex;
  GL::Texture _tex;
  int test;
  TransferFunction* _tf;

  float _step_size;
  GLuint _n_indices;
  std::vector<Vector<float, 4>> _transfer_values_rgba;
  VolumeDisplayMode _display_mode;
  Matrix<float, 4, 4> _bias;
  float _size;

  GL::Program _prog;

  bool _colorMip;
  bool _alphaMip;
  bool _blackWhite;
  bool _depthTerminate;
  bool _depthStart;
  bool _slicing;
  Vector<float, 3> _slicingVector;
  int _transferType;

  int _counter;

  void draw() const;

}; // END class PVolumeDefaultVisualizer

} // END namespace GMlib

// Include PSurfDefaultVisualizer class function implementations
#include "gmpvolumedefaultvisualizer.c"

#endif // GMPVOLUMEDEFAULTVISUALIZER_H

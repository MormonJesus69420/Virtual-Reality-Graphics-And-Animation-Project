

#include "gmpvolumevisualizer.h"
#include "opengl/gmopengl.h"

namespace GMlib {

template <typename T, int n>
PVolumeVisualizer<T, n>::PVolumeVisualizer()
{
}

template <typename T, int n>
PVolumeVisualizer<T, n>::PVolumeVisualizer(const PVolumeVisualizer<T, n>& copy)
{
}

template <typename T, int n>
inline void PVolumeVisualizer<T, n>::fillCubeVBO(GL::VertexBufferObject& vbo, T size)
{
  // Vertices
  GLfloat cube[] = {

    /* 0 */ -0, -0, -0, // Back/Left/Down
    /* 1 */ size, -0, -0,
    /* 2 */ size, size, -0,
    /* 3 */ -0, size, -0,
    /* 4 */ -0, -0, size, // Front/Left/Down
    /* 5 */ size, -0, size,
    /* 6 */ size, size, size,
    /* 7 */ -0, size, size
  };

  vbo.bufferData(24 * sizeof(GLfloat), cube, GL_STATIC_DRAW);
}

template <typename T, int n>
inline void PVolumeVisualizer<T, n>::fillCubeIBO(GL::IndexBufferObject& ibo, GLuint& no_indices)
{

  // Indice Coords
  GLushort cube_indices[] = {

    0, 3, 2, 1, // Back
    2, 3, 7, 6, // Top
    0, 4, 7, 3, // Left
    1, 2, 6, 5, // Right
    4, 5, 6, 7, // Front
    0, 1, 5, 4, // Bottom

  };

  no_indices = 24;

  ibo.bufferData(24 * sizeof(GLushort), cube_indices, GL_STATIC_DRAW);
}

template <typename T, int n>
inline void PVolumeVisualizer<T, n>::fill3DTexture(const DVectorN<Vector<GLfloat, 4>, n>& color, int m1, int m2, int m3, bool closed_u, bool closed_v, bool closed_w, GL::Texture& tex)
{
  std::vector<Vector<float, 4>> colors;

  for (int i = 0; i < m1; i++)
    for (int j = 0; j < m2; j++)
      for (int k = 0; k < m3; k++) {
        colors.push_back(color(i)(j)(k));
      }

  tex.bind();
  tex.texImage3D(0, GL_RGBA, m1, m2, m3, 0, GL_RGBA, GL_FLOAT, colors.data());
  tex.texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  tex.texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  tex.texParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  tex.texParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  tex.texParameterf(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  tex.unbind();
}

template <typename T, int n>
inline void PVolumeVisualizer<T, n>::fillTransferFunction(GL::Texture& transfertex, const std::vector<Vector<float, 4>>& rgba, bool update)
{

  if (update) {
    transfertex.bind();
    transfertex.texSubImage1D(0, 0, 256, GL_RGBA, GL_FLOAT, rgba.data());
    transfertex.unbind();
  }
  else {
    std::cout << "First texture init" << std::endl;
    transfertex.bind();
    transfertex.texImage1D(0, GL_RGBA, 256, 0, GL_RGBA, GL_FLOAT, rgba.data());
    transfertex.texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    transfertex.texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    transfertex.texParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP);
    transfertex.unbind();
  }
}

template <typename T, int n>
inline void PVolumeVisualizer<T, n>::replot(
    const DVectorN<DVectorN<CFDCell<T, n>, 3, T>, 3, T>& /*p*/,
    int /*m1*/, int /*m2*/, int /*m3*/, int /*d1*/, int /*d2*/, int /*d3*/,
    bool /*closed_u*/, bool /*closed_v*/, bool /*closed_w*/
)
{
}

} // namespace GMlib

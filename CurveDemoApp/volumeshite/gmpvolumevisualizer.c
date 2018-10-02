

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

  //    GLfloat texcoord[] = {

  //        /* 0 */     0,    0,    0,      // Back/Left/Down
  //        /* 1 */     1,    0,    0,
  //        /* 2 */     1,    1,    0,
  //        /* 3 */     0,    1,    0,
  //        /* 4 */     0,    0,    1,      // Front/Left/Down
  //        /* 5 */     1,    0,    1,
  //        /* 6 */     1,    1,    1,
  //        /* 7 */     0,    1,    1
  //    };

  vbo.bufferData(24 * sizeof(GLfloat), cube, GL_STATIC_DRAW);
  //    vbo.bufferData( 2*24*sizeof(GLfloat), texcoord, GL_STATIC_DRAW );
  //    tbo.bufferData(24 * sizeof(GLfloat), texcoord, GL_STATIC_DRAW );

  //    // Frame indice coords
  //    GLushort frame_indices [] = {

  //      0,  1,    // x-axis
  //      0,  3,    // y-axis
  //      0,  4,    // z-axis

  //      // Remaining frame
  //      2,  3,
  //      2,  1,
  //      2,  6,

  //      7,  6,
  //      7,  4,
  //      7,  3,

  //      5,  4,
  //      5,  6,
  //      5,  1

  //    };

  //    OGL::bindBo( "std_rep_frame_indices" );
  //    GL_CHECK(glBufferData( GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLushort), frame_indices, GL_STATIC_DRAW ));
  //    OGL::unbindBo( "std_rep_frame_indices" );
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

  //    GLuint indices[] =
  //    {
  //      0, 1, 2,    // Front
  //      2, 1, 3,
  //      4, 0, 6,    // Bottom
  //      6, 0, 2,
  //      7, 5, 6,    // BAck
  //      6, 5, 4,
  //      3, 1, 7,    // Left
  //      7, 1, 5,
  //      4, 5, 0,    // Right
  //      0, 5, 1,
  //      3, 7, 2,    // Top
  //      2, 7, 6
  //    };

  //    no_indices = 36;

  //    ibo.createBufferData(sizeof(GLuint)* no_indices,&indices[0],GL_STATIC_DRAW);
}

template <typename T, int n>
inline void PVolumeVisualizer<T, n>::fill3DTexture(const DVectorN<Vector<GLfloat, 4>, n>& color, int m1, int m2, int m3, bool closed_u, bool closed_v, bool closed_w, GL::Texture& tex)
{
  //    DVectorN< Vector<GLfloat,4>,n> _3DTexture;            // The 3D texture in 3D format.

  //    _3DTexture.setDim(Vector<int,3>(m1,m2,m3));
  std::vector<Vector<float, 4>> colors;

  for (int i = 0; i < m1; i++)
    for (int j = 0; j < m2; j++)
      for (int k = 0; k < m3; k++) {
        colors.push_back(color(i)(j)(k));
        //                std::cout << "Color: " << i << j << k << " -> " << color(i)(j)(k) << std::endl;
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
    //        transfertex.texImage1D(0,GL_RGBA, 256,0,GL_RGBA,GL_FLOAT,rgba.data());
    transfertex.texSubImage1D(0, 0, 256, GL_RGBA, GL_FLOAT, rgba.data());
    //        glTexSubImage1D(GL_TEXTURE_1D, 0,0,256,GL_RGBA,GL_FLOAT,rgba.data());
    //        GL_CHECK(glTexSubImage1D(GL_TEXTURE_1D, 0,0,256,GL_RGBA,GL_FLOAT, rgba[0].getPtr()));
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

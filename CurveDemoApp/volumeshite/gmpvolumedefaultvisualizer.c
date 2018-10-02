#include "gmpvolumedefaultvisualizer.h"

namespace GMlib {

template <typename T, int n>
PVolumeDefaultVisualizer<T, n>::PVolumeDefaultVisualizer()
    : //    _tex(GL_TEXTURE_3D),_transfer_tex(GL_TEXTURE_1D),_depth_tex("scene_depth_map"),
    _step_size(0.001f)
//    _bo_cube("std_rep_cube"), _bo_cube_indices( "std_rep_cube_indices"), _bo_cube_frame_indices( "std_rep_frame_indices" )
{
  _ibo.create();
  _tbo.create();
  _vbo.create();
  _tex.create(GL_TEXTURE_3D);
  _transfer_tex.create(GL_TEXTURE_1D);
  //    bool deptex = _depth_tex.acquire("scene_depth_map");
  bool repcub = _bo_cube.acquire("std_rep_cube");
  bool repcubind = _bo_cube_indices.acquire("std_rep_cube_indices");
  bool repcubframeind = _bo_cube_frame_indices.acquire("std_rep_frame_indices");

  bool found = _prog.acquire("pvolume_cube"); //    this->setRenderProgram( GL::GLProgram("cube_depth_test") );
  std::cout << "Found shader prog pvolume_cube: " << found << std::endl;
  //    std::cout << "Found scene_depth_map: " << deptex << std::endl;
  std::cout << "Found std_rep_cube: " << repcub << std::endl;
  std::cout << "Found std_rep_cube_indices: " << repcubind << std::endl;
  std::cout << "Found std_rep_frame_indices: " << repcubframeind << std::endl;
  _size = 100;
  this->fillCubeVBO(_vbo, _size);
  this->fillCubeVBO(_tbo, 1);
  this->fillCubeIBO(_ibo, _n_indices);
  test = 1;

  _bias.setCol(GMlib::Vector<float, 4>(0.5, 0.0, 0.0, 0.0), 0);
  _bias.setCol(GMlib::Vector<float, 4>(0.0, 0.5, 0.0, 0.0), 1);
  _bias.setCol(GMlib::Vector<float, 4>(0.0, 0.0, 0.5, 0.0), 2);
  _bias.setCol(GMlib::Vector<float, 4>(0.5, 0.5, 0.5, 1.0), 3);

  _colorMip = false;
  _alphaMip = false;
  _blackWhite = false;
  _depthTerminate = false;
  _depthStart = false;
  _slicing = false;
  _slicingVector = GMlib::Vector<float, 3>(0.0, 0.0, 0.0);
  _transferType = 1;
  _display_mode = DM_COLOR;
  _counter = 0;
  _tf = new TransferFunction();
}

//  //TODO Change this
//  template <typename T, int n>
//  PVolumeDefaultVisualizer<T,n>::PVolumeDefaultVisualizer(const PVolumeDefaultVisualizer<T, n> &copy):
//    _vbo(), _ibo(),_tbo(), _tex(GL_TEXTURE_3D),_transfer_tex(GL_TEXTURE_1D),
//    _step_size(0.01f),
//    _bo_cube("std_rep_cube"), _bo_cube_indices( "std_rep_cube_indices"), _bo_cube_frame_indices( "std_rep_frame_indices" ){
//    //    this->setRenderProgram( GL::GLProgram("cube") );
//    //    this->filleCubeVBO(_vbo,_tbo,5);
//    //    this->fillCubeIBO(_ibo,_n_indices);
//  }

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::render(const GMlib::SceneObject* obj, const GMlib::DefaultRenderer* renderer) const
{

  const GMlib::HqMatrix<float, 3>& mvpmat = obj->getModelViewProjectionMatrix(renderer->getCamera());
  const GMlib::HqMatrix<float, 3>& mmat = obj->getMatrix();

  this->glSetDisplayMode();

  _prog.bind();
  {

    glCullFace(GL_FRONT_AND_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_FRONT_AND_BACK);

    // Model view and projection matrices
    _prog.uniform("u_mvpmat", mvpmat);
    _prog.uniform("u_mmat", mmat);
    _prog.uniform("u_bias", _bias);
    _prog.uniform("u_size", _size);

    _prog.uniform("color_mip", _colorMip);
    _prog.uniform("a_mip", _alphaMip);
    _prog.uniform("black_white", _blackWhite);
    _prog.uniform("depth_termination", _depthTerminate);
    _prog.uniform("depth_start", _depthStart); //se mer på denne
    _prog.uniform("transfer_func_type", _transferType);
    _prog.uniform("is_sliced", _slicing);
    _prog.uniform("sX", _slicingVector(0));
    _prog.uniform("sY", _slicingVector(1));
    _prog.uniform("sZ", _slicingVector(2));

    //_prog.setUniform("slice_vec", Point<float,3>(_slicingVector));

    //volume rendering parameters
    GLuint camera_loc = glGetUniformLocation(_prog.getId(), "camera");
    auto cam = renderer->getCamera();
    glUniform3f(camera_loc, cam->getPos()(0), cam->getPos()(1), cam->getPos()(2));
    _prog.uniform("u_stepsize", _step_size);

    // Get vertex and texture attrib locations
    GMlib::GL::AttributeLocation vert_loc = _prog.getAttributeLocation("in_vertex");
    GMlib::GL::AttributeLocation tex_loc = _prog.getAttributeLocation("in_tex");

    //Set 3D texture
    _prog.uniform("samplerDataVolume", _tex, (GLenum)GL_TEXTURE0, 0);

    //Set transfer function
    _prog.uniform("samplerTransferFunction", _transfer_tex, (GLenum)GL_TEXTURE1, 1);

    //Set depth map
    //        if(_depth_tex.isValid())
    //            _prog.uniform("samplerDepthMap", _depth_tex , (GLenum)GL_TEXTURE2,2);
    _prog.uniform("samplerDepthMap", renderer->getDepthTexture(), (GLenum)GL_TEXTURE2, 2);

    // Bind and draw - bytte ut vector med vertex
    _vbo.bind();
    _vbo.enable(vert_loc, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0x0);
    _vbo.unbind();
    _tbo.bind();
    _tbo.enable(tex_loc, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0x0);
    _tbo.unbind();

    draw();

    _tbo.bind();
    _tbo.disable(tex_loc);
    _tbo.unbind();
    _vbo.bind();
    _vbo.disableVertexArrayPointer(vert_loc);
    _vbo.unbind();

    glDisable(GL_BLEND);
  }
  _prog.unbind();
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::renderGeometry(const GMlib::SceneObject* obj, const GMlib::Renderer* renderer, const GMlib::Color& color) const
{

  const GMlib::HqMatrix<float, 3>& mvpmat = obj->getModelViewProjectionMatrix(renderer->getCamera());

  _prog.bind();
  {

    // Model view and projection matrices
    _prog.uniform("u_mvpmat", mvpmat);

    // Get vertex and texture attrib locations
    GMlib::GL::AttributeLocation vert_loc = _prog.getAttributeLocation("in_vertex");

    // Bind and draw - bytte ut vector med vertex
    _vbo.bind();
    _vbo.enable(vert_loc, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0x0);
    _vbo.unbind();
    //        draw();

    _vbo.bind();
    _vbo.disableVertexArrayPointer(vert_loc);
    _vbo.unbind();

    glDisable(GL_BLEND);
  }
  _prog.unbind();
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::draw() const
{

  _ibo.bind();
  _ibo.drawElements(GL_QUADS, _n_indices, GL_UNSIGNED_SHORT, (const GLvoid*)0x0);
  _ibo.unbind();
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::replot(const DVectorN<DVectorN<CFDCell<T, n>, 3, T>, 3, T>& p,
    int m1, int m2, int m3, int d1, int d2, int d3,
    bool closed_u, bool closed_v, bool closed_w)
{

  //        int m1 = closed_u ? p.getDim()(0) -1 : p.getDim()(0);
  //        int m2 = closed_v ? p.getDim()(1) -1 : p.getDim()(1);
  //        int m3 = closed_w ? p.getDim()(2) -1 : p.getDim()(2);

  DVectorN<Vector<GLfloat, 4>, n> color;

  color.setDim(Vector<int, 3>(m1, m2, m3));

  for (int i = 0; i < m1; i++)
    for (int j = 0; j < m2; j++)
      for (int k = 0; k < m3; k++) {
        if (_display_mode == DM_COLOR)
          color[i][j][k] = p(i)(j)(k)(0)(0)(0).getColor();
        if (_display_mode == DM_COLOR_DERU)
          color[i][j][k] = p(i)(j)(k)(1)(0)(0).getColor();

        //                std::cout << color[i][j][k] << std::endl;
      }

  //    color[m1-1][m2-1][m3-1] = Vector<GLfloat,4>(0,0,1,1);

  PVolumeVisualizer<T, n>::fill3DTexture(color, m1, m2, m3, closed_u, closed_v, closed_w, _tex);
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::setTransferValues(const std::vector<Vector<float, 4>>& rgba, bool update)
{
  //    std::cout << "Change transfer function" << std::endl;

  _transfer_values_rgba = rgba;
  this->fillTransferFunction(_transfer_tex, _transfer_values_rgba, update);
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::setTestTexture(unsigned char* volume_)
{
  //#3D Texture
  unsigned int h = 256;
  unsigned int w = 256;
  unsigned int d = 256;

  //    //volume_ = readPVMvolume("../Foot.pvm",&w,&h,&d,&comp,&scaleX,&scaleY,&scaleZ);
  //    volume_ = readPVMvolume(volumeFile_.toAscii().constData(),&w,&h,&d,&comp,&scaleX,&scaleY,&scaleZ);

  //    qDebug() << h << " " << w << " " << d << " " << comp;

  //    if(volume_ == NULL)
  //    {
  //        assert("INGEN FIL FEIL ERROR 1337/42 ");
  //    }
  if (volume_ == NULL)
    std::cout << "Volume is zero";

  _tex.bind();
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, w, h, d, 0, GL_RED, GL_UNSIGNED_BYTE, volume_);
  _tex.unbind();
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::setVolumeDisplayMode(VolumeDisplayMode mode)
{
  _display_mode = mode;
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::setShaders(bool colorMip, bool alphaMip, bool blackWhite, bool depthTerminate, bool depthStart, bool slicing)
{
  _colorMip = colorMip;
  _alphaMip = alphaMip;
  _blackWhite = blackWhite;
  _depthTerminate = depthTerminate;
  _depthStart = depthStart;
  _slicing = slicing;
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::setSlicingVector(Vector<float, 3> slicingVector)
{
  _slicingVector = slicingVector;
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::setSlicingVector(float x, float y, float z)
{
  setSlicingVector(Vector<float, 3>(x, y, z));
}

template <typename T, int n>
inline void PVolumeDefaultVisualizer<T, n>::setTransferType(int index)
{
  _transferType = index;
}

template <typename T, int n>
void PVolumeDefaultVisualizer<T, n>::toggleTransferValues()
{
  _tf->toggleTransfer();
  //    _tf->toggleRed();
  //    _tf->toggleGreen();
  //    _tf->toggleBlue();
  setTransferValues(_tf->getTransferValues());
}

template <typename T, int n>
void PVolumeDefaultVisualizer<T, n>::updateTransferValues(bool update)
{
  setTransferValues(_tf->getTransferValues(), update);
}

} // namespace GMlib

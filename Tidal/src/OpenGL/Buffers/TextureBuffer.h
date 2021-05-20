#ifndef TIDAL_TIDAL_SRC_OPENGL_BUFFERS_TEXTUREBUFFER_H_
#define TIDAL_TIDAL_SRC_OPENGL_BUFFERS_TEXTUREBUFFER_H_

#include "Utility/Profiler.h"

namespace GL {
class TextureBuffer {
 private:
  unsigned int bufferID;
  unsigned int bufferTextureID;
  GLenum textureUnit;
  GLenum format;

 public:
  TextureBuffer(GLenum _textureUnit, GLenum format);
  virtual ~TextureBuffer();

  void BindVBO();
  void Bind();

  glm::vec3 *Pointer();
  void PointerRelease();
};
}
#endif //TIDAL_TIDAL_SRC_OPENGL_BUFFERS_TEXTUREBUFFER_H_

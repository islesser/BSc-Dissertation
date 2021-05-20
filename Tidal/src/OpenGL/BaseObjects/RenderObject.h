#ifndef TIDAL_TIDAL_SRC_OPENGL_BASEOBJECTS_RENDEROBJECT_H_
#define TIDAL_TIDAL_SRC_OPENGL_BASEOBJECTS_RENDEROBJECT_H_

#include "Object.h"
#include "Renderable.h"

namespace GL {
class RenderObject : public Object, public Renderable {
 public:
  RenderObject(float x,
			   float y,
			   float z,
			   float w,
			   float h,
			   float d,
			   PROGRAM_TYPE _programType,
			   GLenum _mode = GL_TRIANGLES)
	  : Object(x, y, z, w, h, d), Renderable(_programType, _mode) {}
  virtual ~RenderObject() = default;
};
}

#endif //TIDAL_TIDAL_SRC_OPENGL_BASEOBJECTS_RENDEROBJECT_H_

#ifndef PARTICLE_PARTICLE_SRC_OPENGL_PRIMITIVES_PLANE_H_
#define PARTICLE_PARTICLE_SRC_OPENGL_PRIMITIVES_PLANE_H_

#include "Vertex.h"
#include "OpenGL/BaseObjects/Object.h"
#include "OpenGL/BaseObjects/Renderable.h"
#include "OpenGL/Programs/ProgramManager.h"

namespace GL {
class Plane : public Object, public Renderable {
 protected:
  float resolution;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

 public:
  Plane(float x, float y, float z, float width, float height, float depth, float _resolution = 20);
  virtual ~Plane();
  virtual void Generate();
};
}
#endif //PARTICLE_PARTICLE_SRC_OPENGL_PRIMITIVES_PLANE_H_

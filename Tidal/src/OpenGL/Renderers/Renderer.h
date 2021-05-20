#ifndef TIDAL_TIDAL_SRC_OPENGL_RENDERERS_RENDERER_H_
#define TIDAL_TIDAL_SRC_OPENGL_RENDERERS_RENDERER_H_

#include "OpenGL/Programs/ProgramManager.h"
#include "Core/Window/Window.h"
#include "OpenGL/BaseObjects/Renderable.h"
#include "Utility/Profiler.h"
#include "Core/GUI/GUILayer.h"
#include "OpenGL/Datastores/Datastore.h"

namespace GL {

// Manages the rendering process such as handling draw calls and manages related data
// Ie. Lighting variables.

class Renderer {
 protected:
  std::shared_ptr<ProgramManager> programs; // Shader program cache
  float AmbientStrength = 0.2f; // Ambient light strength
  float SpecularStrength = 0.0f; // Specular light strength
  glm::vec3 LightPosition = glm::vec3(150.0f, 500.0f, 300.0f); // Primary light position

 public:
  Renderer() {}
  virtual ~Renderer() {}

  void GUIElement(std::string name);
  virtual bool Init(GUILayer *gui, std::shared_ptr<ProgramManager> _programs);
  virtual void Draw(double dt, Renderable *r);
  virtual void Draw(double dt, std::shared_ptr<Datastore> datastore);
};

}

#endif //TIDAL_TIDAL_SRC_OPENGL_RENDERERS_RENDERER_H_

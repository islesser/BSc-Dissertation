#ifndef TIDAL_TIDAL_SRC_CORE_WINDOW_MOUSESELECTOR_H_
#define TIDAL_TIDAL_SRC_CORE_WINDOW_MOUSESELECTOR_H_

#include "Camera.h"
#include "OpenGL/BaseObjects/Object.h"

// A class that converts the mouse position to world coordinates and stores
// the position.
class MouseSelector {
 private:
  Camera *camera; // Pointer to the camera.
  glm::vec3 ray; // Ray intersection point.

 public:
  MouseSelector() {}
  MouseSelector(Camera *camera) : camera(camera) {};
  virtual ~MouseSelector() {};

  void Update(float mouseX, float mouseY);
  void Update(glm::ivec2 objectPosition);

  glm::vec3 ToWorld(glm::vec4 eye);
  glm::vec4 ToEye(glm::vec4 clip);
  glm::vec2 ToNDC(float mouseX, float mouseY);

  // Return the ray value
  inline glm::vec3 &Ray() { return ray; }
};

#endif //TIDAL_TIDAL_SRC_CORE_WINDOW_MOUSESELECTOR_H_

#ifndef TIDAL_TIDAL_SRC_SIMULATION_PILLAR_H_
#define TIDAL_TIDAL_SRC_SIMULATION_PILLAR_H_

#include "OpenGL/Primitives/Cube.h"
#include "Core/Window/Window.h"
#include "ComputerVision/WebcamCursor.h"

// An object to demonstrate interactive objects within the scene.
class Pillar : public FlatCube {
 private:
  glm::vec3 Min; // Object minimum bounds.
  glm::vec3 Max; // Object maximum bounds.
  bool selectedLastFrame = false; // Track if selected on the previous frame.

 public:
  Pillar(float x, float y, float z, float width, float height, float depth);
  virtual ~Pillar() {};
  bool Update(Window *window);
  bool IntersectMouseSelector(Window *window);
  bool IntersectWebcamCursor(WebcamCursor *wcc);
};

#endif //TIDAL_TIDAL_SRC_SIMULATION_PILLAR_H_

#ifndef TIDAL_TIDAL_SRC_SIMULATION_WATER_H_
#define TIDAL_TIDAL_SRC_SIMULATION_WATER_H_

#include "OpenGL/Primitives/Plane.h"
#include "OpenGL/Programs/ProgramManager.h"
#include "Core/GUI/GUILayer.h"
#include "OpenGL/Buffers/TextureBuffer.h"
#include "Simulation/Terrain.h"

// Generates and manages water vertices and related variables through uniform passing
// and GUI exposure.
class Water : public GL::Plane {
 private:
  float Min = 0.5f; // Minimum depth factor
  float Max = 2.0f; // Maximum depth factor
  float foamHeight = 51.5f; // Height threshold before considered foam
  float shorethreshold = 3.9f; // Distance from shoreline threshold

  std::vector<Vertex> vertices2; // used for the CPU implementation test

  glm::vec4 wave0 = glm::vec4(0.09f, 0.71f, 0.06f, 85.0f); // First Gerstner wave
  glm::vec4 wave1 = glm::vec4(-0.01f, 0.45f, 0.03f, 30.0f); // Second Gerstner wave
  glm::vec4 wave2 = glm::vec4(0.18f, 0.32f, 0.06f, 40.0f); // Third Gerstner wave
  glm::vec4 OceanColour = glm::vec4(0.21f, 0.55f, 0.77f, 0.0f); // Default ocean colour.

 public:
  Water(GUILayer *gui, float x, float y, float z, float w, float h, float d, float resolution = 20);
  virtual ~Water() {};

  virtual void Begin(GL::Program *program) override;
  void GUIElement(std::string name);
  virtual void Generate() override;
  void Update(std::vector<float> &heights);
};

#endif //TIDAL_TIDAL_SRC_SIMULATION_WATER_H_

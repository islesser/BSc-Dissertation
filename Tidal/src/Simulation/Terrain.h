#ifndef PARTICLE_PARTICLE_SRC_OPENGL_BASEOBJECTS_TERRAIN_H_
#define PARTICLE_PARTICLE_SRC_OPENGL_BASEOBJECTS_TERRAIN_H_

#include "Utility/Random/Perlin.h"
#include "OpenGL/Primitives/Plane.h"
#include "Core/GUI/GUILayer.h"
#include "Simulation/SceneHeightMap.h"

// Generates terrain vertices and manages passing uniforms, modifying values through
// debug GUI etc.
class Terrain : public GL::Plane {
 private:
  std::shared_ptr<Utils::Perlin> perlin; // Perlin noise generator
  std::vector<float> maxHeightThresholds; // Min heights (along z-axis)
  std::vector<float> minHeightThresholds; // Max heights (along z-axis)
  HeightData heightData; // Stored height data

  struct Colours {
	std::vector<float> threshold = {0.23f, 0.29f, 0.42f, 0.54f, 0.63f};
	glm::vec4 colour1 = glm::vec4(54 / 255.0f, 140 / 255.0f, 196 / 255.0f, 1.0f);
	glm::vec4 colour2 = glm::vec4(176 / 255.0f, 183 / 255.0f, 11 / 255.0f, 1.0f);
	glm::vec4 colour3 = glm::vec4(133 / 255.0f, 221 / 255.0f, 110 / 255.0f, 1.0);
	glm::vec4 colour4 = glm::vec4(107 / 255.0f, 176 / 255.0f, 77 / 255.0f, 1.0);
	glm::vec4 colour5 = glm::vec4(87 / 255.0f, 102 / 255.0f, 115 / 255.0f, 1.0);
	glm::vec4 def = glm::vec4(1.0, 1.0, 1.0, 1.0);
  } colours; // Colors to use (based on height of terrain)

 public:
  Terrain(GUILayer *gui,
		  float x,
		  float y,
		  float z,
		  float width,
		  float height,
		  float depth,
		  std::shared_ptr<Utils::Perlin> _perlin,
		  float _resolution = 20);
  virtual ~Terrain() {}

  virtual void Generate() override;
  virtual void Begin(GL::Program *program) override;
  virtual void GenerateHeightThresholds();
  void GUIElement(std::string name);
  glm::vec4 GetColourFromHeight(float height);
  glm::vec3 CalculateNormal(unsigned int x, unsigned int z);
  inline HeightData *HeightMap() { return &heightData; }
};
#endif //PARTICLE_PARTICLE_SRC_OPENGL_BASEOBJECTS_TERRAIN_H_

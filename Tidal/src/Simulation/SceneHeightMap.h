#ifndef TIDAL_TIDAL_SRC_SIMULATION_SCENEHEIGHTMAP_H_
#define TIDAL_TIDAL_SRC_SIMULATION_SCENEHEIGHTMAP_H_

#include "OpenGL/Buffers/TextureBuffer.h"
#include "OpenGL/BaseObjects/Object.h"

// Encapsulates height data.
struct HeightData {
  std::vector<float> heights; // Heights (1D)
  std::vector<std::vector<float>> heights2D; // 2D Representation of the heights
  glm::ivec3 dimensions; // Dimensions
  float yOffset; // Starting y-height
  float resolution; // Size of a cell
};

// Handles functions related to the generation of height fields and conversion into
// texture buffers.
class SceneHeightMap {
 private:
  HeightData sceneheightdata; // Scene: Terrain and objects
  HeightData terrainheightdata; // Terrain: Terrain only

  GL::TextureBuffer *scene; // Scene height map texture
  GL::TextureBuffer *terrain; // Terrain height map texture

 public:
  SceneHeightMap();
  virtual ~SceneHeightMap();

  void Init(std::vector<Object *> objects, float waterlevel);
  void RefreshSceneHeightMap(std::vector<Object *> objects, float waterlevel);

  // Inline getters & setters.
  inline void SetTerrain(HeightData *terrain) { terrainheightdata = *terrain; }
  inline HeightData *Terrain() { return &terrainheightdata; }
  inline HeightData *Scene() { return &sceneheightdata; }


  // Binds the textures
  inline void Bind() {
	terrain->Bind();
	scene->Bind();
  }
};

#endif //TIDAL_TIDAL_SRC_SIMULATION_SCENEHEIGHTMAP_H_

#include "PCHeader.h"
#include "SceneHeightMap.h"

// Create textures
SceneHeightMap::SceneHeightMap() {
  terrain = new GL::TextureBuffer(GL_TEXTURE0, GL_R32F);
  scene = new GL::TextureBuffer(GL_TEXTURE1, GL_R32F);
}

// Delete the textures
SceneHeightMap::~SceneHeightMap() {
  delete scene;
  delete terrain;
}

// Initializes the height map be populating the data and adding height data from objects.
void SceneHeightMap::Init(std::vector<Object *> objects, float waterlevel) {
  sceneheightdata.dimensions = terrainheightdata.dimensions;
  sceneheightdata.yOffset = 0.0f;
  sceneheightdata.resolution = terrainheightdata.resolution;

  // Create the scene height map
  RefreshSceneHeightMap(objects, waterlevel);

  // Populate terrain texture
  terrain->BindVBO();
  GLCheck(glBufferData(GL_TEXTURE_BUFFER,
					   sizeof(float) * terrainheightdata.heights.size(),
					   terrainheightdata.heights.data(),
					   GL_DYNAMIC_DRAW));
  auto sPtr = terrain->Pointer();
  memcpy(sPtr, terrainheightdata.heights.data(), sizeof(float) * terrainheightdata.heights.size());
  terrain->PointerRelease();


}

// Refresh the scene height map, in the event that an object in the scene is moved or changed in some way/
void SceneHeightMap::RefreshSceneHeightMap(std::vector<Object *> objects, float waterlevel) {

  // Start by clearing to the terrain heights.
  sceneheightdata.heights = terrainheightdata.heights;

  // Indexing lambda
  auto Index = [&](int x, int z) { return z * sceneheightdata.dimensions.x + x; };

  // Populate the scene data using object heights
  for (int i = 0; i < objects.size(); ++i) {
	auto objectpos = objects[i]->Position();
	auto objectdims = objects[i]->Dimensions();

	int padding = 1;

	// Create a padding around the objects exterior and calculate the bounds
	int xp = std::max((int) (objectpos.x - padding), 0);
	int zp = std::max((int) (objectpos.z - padding), 0);
	int xpm = std::min((int) (objectpos.x + padding + objectdims.x), (int) (sceneheightdata.dimensions.x - 1));
	int zpm = std::min((int) (objectpos.z + padding+ objectdims.z), (int) (sceneheightdata.dimensions.z - 1));

	// Update the data based on the calculated bounds.
	for (int z = zp; z < zpm; ++z) {
	  for (int x = xp; x < xpm; ++x) {
		int index = Index(x, z);
		float h = std::max(objectdims.y, terrainheightdata.heights[index]);
		sceneheightdata.heights[index] = h;
	  }
	}
  }

  // Populate the buffers
  scene->BindVBO();
  GLCheck(glBufferData(GL_TEXTURE_BUFFER,
					   sizeof(float) * sceneheightdata.heights.size(),
					   sceneheightdata.heights.data(),
					   GL_DYNAMIC_DRAW));
  auto tPtr = scene->Pointer();
  memcpy(tPtr, sceneheightdata.heights.data(), sizeof(float) * sceneheightdata.heights.size());
  scene->PointerRelease();
}

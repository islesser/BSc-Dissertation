#ifndef PARTICLE_PARTICLE_SRC_OPENGL_PRIMITIVES_OBJECT_H_
#define PARTICLE_PARTICLE_SRC_OPENGL_PRIMITIVES_OBJECT_H_

#include "OpenGL/Datastores/Datastore.h"

struct SceneBounds {
  glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 size = glm::vec3(0.0f, 0.0f, 0.0f);
};

class Object {
 protected:
  glm::vec3 position;
  glm::vec3 dimensions;
  SceneBounds scenebounds;

 public:
  Object(float x, float y, float z, float w, float h, float d)
	  : position(glm::vec3(x, y, z)), dimensions(glm::vec3(w, h, d)) {}
  virtual ~Object() {}

  const glm::vec3 &Dimensions() const { return dimensions; }
  void SetDimensions(glm::ivec3 to) { dimensions = to; }
  void AddWidth(int add) { dimensions.x += add; }
  void AddHeight(int add) { dimensions.y += add; }
  void AddDepth(int add) { dimensions.z += add; }

  const glm::vec3 &Position() const { return position; }
  void SetPosition(glm::ivec3 to) { position = to; }
  void AddX(int add) { position.x += add; }
  void AddY(int add) { position.y += add; }
  void AddZ(int add) { position.z += add; }

  void SetSceneBounds(SceneBounds &to) { scenebounds = to; }
  void CheckSceneBounds() {
	// X Bounds
	if (position.x < 0.0f) position.x = scenebounds.origin.x;
	else if (position.x > scenebounds.origin.x + scenebounds.size.x)
	  position.x = scenebounds.origin.x + scenebounds.size.x;

	// Y Bounds
	if (position.y < 0.0f) position.y = scenebounds.origin.y;
	else if (position.y > scenebounds.origin.y + scenebounds.size.y)
	  position.y = scenebounds.origin.y + scenebounds.size.y;

	// Z Bounds
	if (position.z < 0.0f) position.z = scenebounds.origin.z;
	else if (position.z > scenebounds.origin.z + scenebounds.size.z)
	  position.z = scenebounds.origin.z + scenebounds.size.z;
  }

  void SubmitDebugOutline(std::shared_ptr<GL::Datastore> datastore) {
	float x = position.x;
	float y = position.y;
	float z = position.z;

	int X = dimensions.x;
	int Y = dimensions.y;
	int Z = dimensions.z;

	// Front
	datastore->Add({glm::vec3(x, y, z)}, {glm::vec3(x + X, y, z)}); // Bottom
	datastore->Add({glm::vec3(x, y, z)}, {glm::vec3(0.0f, y + Y, z)}); // Left
	datastore->Add({glm::vec3(x, y + Y, z)}, {glm::vec3(x + X, y + Y, z)}); // Top
	datastore->Add({glm::vec3(x + X, y, z)}, {glm::vec3(x + X, y + Y, z)}); // Right

	// Back
	datastore->Add({glm::vec3(x, y, z + Z)}, {glm::vec3(x + X, y, z + Z)}); // Bottom
	datastore->Add({glm::vec3(x, y, z + Z)}, {glm::vec3(x, y + Y, z + Z)}); // Left
	datastore->Add({glm::vec3(x, y + Y, z + Z)}, {glm::vec3(X, y + Y, z + Z)}); // Top
	datastore->Add({glm::vec3(x + X, y, z + Z)}, {glm::vec3(X, y + Y, z + Z)}); // Right

	// Connections
	datastore->Add({glm::vec3(x, y + Y, z)}, {glm::vec3(x, y + Y, z + Z)}); // Top Left
	datastore->Add({glm::vec3(x + X, y + Y, z)}, {glm::vec3(x + X, y + Y, z + Z)}); // Top Right
	datastore->Add({glm::vec3(x, y, z)}, {glm::vec3(x, y, z + Z)}); // Bottom Left
	datastore->Add({glm::vec3(x + X, y, z)}, {glm::vec3(x + X, y, z + Z)}); // Bottom Right
  }
};

#endif //PARTICLE_PARTICLE_SRC_OPENGL_PRIMITIVES_OBJECT_H_

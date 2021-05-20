#include "PCHeader.h"
#include "Plane.h"

namespace GL {
Plane::Plane(float x, float y, float z, float width, float height, float depth, float _resolution)
	: Object(x, y, z, width, height, depth), resolution(_resolution), Renderable(RENDER_MESH) {
}

Plane::~Plane() {
}

void Plane::Generate() {
  int cellsX = dimensions.x / resolution;
  int cellsZ = dimensions.z / resolution;

  // Create Vertices
  glm::vec4 COLOUR = glm::vec4(0.21f, 0.55f, 0.77f, 1.0f);
  for (float z = 0; z < cellsZ; ++z) {
	for (float x = 0; x < cellsX; ++x) {
	  int xp = position.x + x * resolution;
	  int zp = position.z + z * resolution;

	  Vertex vertex = {
		  {xp, position.y, zp},
		  COLOUR,
		  {0.0f, 0.0f, 0.0f}
	  };

	  vertices.push_back(vertex);
	}
  }

  for (int z = 0; z < cellsZ - 1; ++z) {
	int Z = z * (cellsX);
	int ZN = (z + 1) * (cellsX);

	for (int x = 0; x < cellsX - 1; ++x) {

	  indices.push_back(Z + x); // A
	  indices.push_back(ZN + x + 1); // C
	  indices.push_back(ZN + x); // D

	  indices.push_back(ZN + x + 1); // C
	  indices.push_back(Z + x); // A
	  indices.push_back(Z + x + 1); // B
	}
  }

  for (int i = 0; i < indices.size(); i += 3) {
	unsigned int ia = indices[i];
	unsigned int ib = indices[i + 1];
	unsigned int ic = indices[i + 2];

	glm::vec3 normal = glm::normalize(glm::cross(
		vertices[ib].position - vertices[ia].position,
		vertices[ic].position - vertices[ia].position));
	vertices[ia].normal = vertices[ib].normal = vertices[ic].normal = normal;
  }

  __Init(vertices, indices);
}
}
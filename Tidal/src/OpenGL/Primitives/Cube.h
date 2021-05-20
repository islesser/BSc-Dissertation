#ifndef CUBE_H
#define CUBE_H

#include "OpenGL/BaseObjects/RenderObject.h"
#include "Vertex.h"

class Cube : public GL::RenderObject {
 public:
  Cube(float x, float y, float z, float width, float height, float depth, glm::vec4 colour = Colour::WHITE)
	  : RenderObject(x, y, z, width, height, depth, GL::RENDER_MESH), colour(colour) {
  }

  virtual ~Cube() {}
  virtual void Begin(GL::Program *program) override {
	Bind();
  }

  virtual void Generate() {
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
		position.x + dimensions.x * 0.5f,
		position.y + dimensions.y * 0.5f,
		position.z + dimensions.z * 0.5f)) * /* Rotate here if needed */
		glm::scale(glm::mat4(1.0f), dimensions);

	__Init(vertices, indices);
  }

 private:
  glm::vec4 colour = Colour::BLUE;

  std::vector<Vertex> vertices = {
	  {{-0.5f, -0.5f, -0.5f}, colour, {-0.5f, -0.5f, -0.5f}},
	  {{0.5f, -0.5f, -0.5f}, colour, {0.5f, -0.5f, -0.5f}},
	  {{0.5f, 0.5f, -0.5f}, colour, {0.5f, 0.5f, -0.5f}},
	  {{-0.5f, 0.5f, -0.5f}, colour, {-0.5f, 0.5f, -0.5f}},

	  {{-0.5f, -0.5f, 0.5f}, colour, {-0.5f, -0.5f, 0.5f}},
	  {{0.5f, -0.5f, 0.5f}, colour, {0.5f, -0.5f, 0.5f}},
	  {{0.5f, 0.5f, 0.5f}, colour, {0.5f, 0.5f, 0.5f}},
	  {{-0.5f, 0.5f, 0.5f}, colour, {-0.5f, 0.5f, 0.5f}},
  };

  std::vector<unsigned int> indices =
	  {
		  0, 1, 3, 3, 1, 2,
		  1, 5, 2, 2, 5, 6,
		  5, 4, 6, 6, 4, 7,
		  4, 0, 7, 7, 0, 3,
		  3, 2, 7, 7, 2, 6,
		  4, 5, 0, 0, 5, 1};
};

class FlatCube : public GL::RenderObject {
 public:
  FlatCube(float x, float y, float z, float width, float height, float depth, glm::vec4 colour = Colour::BLUE)
	  : RenderObject(x, y, z, width, height, depth, GL::RENDER_MESH), colour(colour) {
  }

  virtual ~FlatCube() {}

  virtual void Begin(GL::Program *program) override {
	Bind();
  }

  virtual void Generate() {
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
		position.x + dimensions.x * 0.5f,
		position.y + dimensions.y * 0.5f,
		position.z + dimensions.z * 0.5f)) * /* Rotate here if needed */
		glm::scale(glm::mat4(1.0f), dimensions);

	__Init(vertices, indices);
  }

 private:
  glm::vec4 colour = Colour::WHITE;
  std::vector<Vertex> vertices = {
	  {{-0.5f, -0.5f, -0.5f}, colour, {0.0f, 0.0f, -0.5f}},
	  {{0.5f, -0.5f, -0.5f}, colour, {0.0f, 0.0f, -0.5f}},
	  {{0.5f, 0.5f, -0.5f}, colour, {0.0f, 0.0f, -0.5f}},
	  {{0.5f, 0.5f, -0.5f}, colour, {0.0f, 0.0f, -0.5f}},
	  {{-0.5f, 0.5f, -0.5f}, colour, {0.0f, 0.0f, -0.5f}},
	  {{-0.5f, -0.5f, -0.5f}, colour, {0.0f, 0.0f, -0.5f}},

	  {{-0.5f, -0.5f, 0.5f}, colour, {0.0f, 0.0f, 0.5f}},
	  {{0.5f, -0.5f, 0.5f}, colour, {0.0f, 0.0f, 0.5f}},
	  {{0.5f, 0.5f, 0.5f}, colour, {0.0f, 0.0f, 0.5f}},
	  {{0.5f, 0.5f, 0.5f}, colour, {0.0f, 0.0f, 0.5f}},
	  {{-0.5f, 0.5f, 0.5f}, colour, {0.0f, 0.0f, 0.5f}},
	  {{-0.5f, -0.5f, 0.5f}, colour, {0.0f, 0.0f, 0.5f}},

	  {{-0.5f, 0.5f, 0.5f}, colour, {-0.5f, 0.0f, 0.0f}},
	  {{-0.5f, 0.5f, -0.5f}, colour, {-0.5f, 0.0f, 0.0f}},
	  {{-0.5f, -0.5f, -0.5f}, colour, {-0.5f, 0.0f, 0.0f}},
	  {{-0.5f, -0.5f, -0.5f}, colour, {-0.5f, 0.0f, 0.0f}},
	  {{-0.5f, -0.5f, 0.5f}, colour, {-0.5f, 0.0f, 0.0f}},
	  {{-0.5f, 0.5f, 0.5f}, colour, {-0.5f, 0.0f, 0.0f}},

	  {{0.5f, 0.5f, 0.5f}, colour, {0.5f, 0.0f, 0.0f}},
	  {{0.5f, 0.5f, -0.5f}, colour, {0.5f, 0.0f, 0.0f}},
	  {{0.5f, -0.5f, -0.5f}, colour, {0.5f, 0.0f, 0.0f}},
	  {{0.5f, -0.5f, -0.5f}, colour, {0.5f, 0.0f, 0.0f}},
	  {{0.5f, -0.5f, 0.5f}, colour, {0.5f, 0.0f, 0.0f}},
	  {{0.5f, 0.5f, 0.5f}, colour, {0.5f, 0.0f, 0.0f}},

	  {{-0.5f, -0.5f, -0.5f}, colour, {0.0f, -0.5f, 0.0f}},
	  {{0.5f, -0.5f, -0.5f}, colour, {0.0f, -0.5f, 0.0f}},
	  {{0.5f, -0.5f, 0.5f}, colour, {0.0f, -0.5f, 0.0f}},
	  {{0.5f, -0.5f, 0.5f}, colour, {0.0f, -0.5f, 0.0f}},
	  {{-0.5f, -0.5f, 0.5f}, colour, {0.0f, -0.5f, 0.0f}},
	  {{-0.5f, -0.5f, -0.5f}, colour, {0.0f, -0.5f, 0.0f}},

	  {{-0.5f, 0.5f, -0.5f}, colour, {0.0f, 0.5f, 0.0f}},
	  {{0.5f, 0.5f, -0.5f}, colour, {0.0f, 0.5f, 0.0f}},
	  {{0.5f, 0.5f, 0.5f}, colour, {0.0f, 0.5f, 0.0f}},
	  {{0.5f, 0.5f, 0.5f}, colour, {0.0f, 0.5f, 0.0f}},
	  {{-0.5f, 0.5f, 0.5f}, colour, {0.0f, 0.5f, 0.0f}},
	  {{-0.5f, 0.5f, -0.5f}, colour, {0.0f, 0.5f, 0.0f}}};

  std::vector<unsigned int> indices;
};

#endif /* CUBE_H */

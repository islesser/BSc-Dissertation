#ifndef VERTEX_H
#define VERTEX_H

#include "../Buffers/VBOLayout.h"

// Color presets
namespace Colour {
static const glm::vec4 &WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
static const glm::vec4 &BLACK = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
static const glm::vec4 &RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
static const glm::vec4 &GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
static const glm::vec4 &BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
static const glm::vec4 &GREY = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
static const glm::vec4 &PINK = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

} // namespace Colour

// Data for each individual vertex.
struct Vertex {
  glm::vec3 position;
  glm::vec4 colour = Colour::WHITE;
  glm::vec3 normal = {0.0f, 0.0f, 0.0f};

  // Used to get the layout of the vertex for setting up
  // VAO and VBO
  static GL::VBOLayout GetLayout() {
	GL::VBOLayout layout;
	layout.push<float>(3, 0); // Position
	layout.push<float>(4, 1); // Colour
	layout.push<float>(3, 1); // Normal
	return layout;
  }
};

#endif /* VERTEX_H */
#ifndef PARTICLE_PARTICLE_SRC_OPENGL_RENDERABLES_RENDERABLE_H_
#define PARTICLE_PARTICLE_SRC_OPENGL_RENDERABLES_RENDERABLE_H_

#include "OpenGL/Buffers/VertexArray.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Programs/ProgramManager.h"

namespace GL {
class Renderable {
 protected:
  unsigned int countElements;
  unsigned int countIndices;
  GLenum mode;
  PROGRAM_TYPE programType;
  glm::vec4 tint = {1.0f, 1.0f, 1.0f, 1.0f};

  VertexArray *vao;
  VertexBuffer<Vertex> *vbo;
  unsigned int IBO;

  glm::mat4 modelMatrix = glm::mat4(1.0f);

 public:
  Renderable(PROGRAM_TYPE _programType, GLenum _mode = GL_TRIANGLES) : programType(_programType), mode(_mode) {}

  virtual ~Renderable() {
	delete vao;
	delete vbo;
	GLCheck(glDeleteBuffers(1, &IBO));
  }

  virtual void Begin(Program *program) = 0;

  virtual void Bind() {
	vao->Bind();
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
  }

  inline glm::vec4 Tint() const { return tint; }
  inline void SetTint(glm::vec4 to) { tint = to; }
  inline unsigned int Vertices() { return countElements; }
  inline unsigned int Indices() { return countIndices; }
  inline glm::mat4 Model() { return modelMatrix; }

  inline VertexArray *VAO() { return vao; }
  inline VertexBuffer<Vertex> *VBO() { return vbo; }

  inline Vertex *Pointer() { return vbo->Pointer(); }
  inline void PointerRelease() { vbo->PointerRelease(); }
  inline unsigned int Mode() { return mode; }
  inline PROGRAM_TYPE Program() { return programType; }

  inline void Set(PROGRAM_TYPE _programType, GLenum _mode = GL_TRIANGLES) {
	this->programType = _programType;
	this->mode = _mode;
  }

 protected:
  virtual void __Init(std::vector<Vertex> &data, std::vector<unsigned int> &indices) {
	countElements = data.size();
	countIndices = indices.size();

	vao = new VertexArray();
	vao->Init();

	vbo = new VertexBuffer<Vertex>(GL_DYNAMIC_DRAW);
	vbo->Init(data);

	vao->SetLayout(Vertex::GetLayout());

	GLCheck(glGenBuffers(1, &IBO));
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, countIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
  }

  virtual void __ChangeData(std::vector<Vertex> &data, std::vector<unsigned int> &indices) {
	countElements = data.size();
	countIndices = indices.size();

	vbo->Bind();
	GLCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * countElements, data.data(), GL_DYNAMIC_DRAW));

	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
						 countIndices * sizeof(unsigned int),
						 indices.data(),
						 GL_DYNAMIC_DRAW));
  }
};
}

#endif //PARTICLE_PARTICLE_SRC_OPENGL_RENDERABLES_RENDERABLE_H_

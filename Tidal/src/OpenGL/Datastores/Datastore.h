#ifndef TIDAL_TIDAL_SRC_OPENGL_DATASTORES_DATASTORE_H_
#define TIDAL_TIDAL_SRC_OPENGL_DATASTORES_DATASTORE_H_

#include "../Primitives/Vertex.h"
#include "../Buffers/VertexArray.h"
#include "../Programs/ProgramManager.h"

#include <math.h>

namespace GL {
struct HeightData;

// Stores vertex data for multiple objects
class Datastore {
 protected:
  std::vector<Vertex> elements; // First Vertex List
  std::vector<Vertex> elements2; // Second Vertex List
  std::mutex mutex;

  std::unique_ptr<VertexBuffer<Vertex>> VB; // Vertex Buffer Object
  std::unique_ptr<VertexArray> VA; // Vertex Array Object
  PROGRAM_TYPE programType; // Program to use
  GLenum mode; // Draw Mode (I.e. GL_LINES)

 private:
 public:
  Datastore(PROGRAM_TYPE _programType, GLenum _mode = GL_LINES) : programType(_programType), mode(_mode) {}

  virtual ~Datastore() {
  }

  // Bind the program
  virtual void Begin(Program *program) {
	Bind();
  }

  // Add a single vertex to the store (thread safe)
  void Add(Vertex v) {
	std::lock_guard<std::mutex> lockGuard(mutex);
	elements.push_back(v);
  }

  // Add two vertices (a line) to the store (thread safe)
  void Add(Vertex v0, Vertex v1) {
	elements.clear();
	std::lock_guard<std::mutex> lockGuard(mutex);
	elements.push_back(v0);
	elements.push_back(v1);
  }

  bool Init() {
	VA = std::make_unique<VertexArray>();
	VA->Init();

	VB = std::make_unique<VertexBuffer<Vertex>>(GL_DYNAMIC_DRAW);
	VB->Init(sizeof(Vertex) * elements.size());

	VA->SetLayout(Vertex::GetLayout());

	// Here we separate the init & data phases as many data sources for these elements.
	VB->Data(elements);

	elements2.resize(elements.size());
	std::copy(elements.begin(), elements.end(), elements2.begin());

	return true;
  }

  bool Update() {
	VB->Data(elements2);
	return true;
  }

  void Bind() {
	VA->Bind();
  }

  inline std::vector<Vertex> Elements() { return elements; }
  int Size() { return elements.size(); }
  inline GLenum Mode() { return mode; }
  inline PROGRAM_TYPE Program() { return programType; }

  void Gerstner(std::vector<float> &heights);

};

}

#endif //TIDAL_TIDAL_SRC_OPENGL_DATASTORES_DATASTORE_H_
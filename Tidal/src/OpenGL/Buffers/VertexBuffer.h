#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "../Primitives/Vertex.h"

namespace GL {

template<class T>
struct VBPointer {
  T *start;
  T *it;
  unsigned int size;
};

template<class T>
class VertexBuffer {
 private:
  unsigned int ID;
  unsigned long size;
  GLenum usage;

 public:
  VertexBuffer(GLenum _usage);
  ~VertexBuffer();

  void Init(const unsigned long &_size);
  void Init(std::vector<T> &data);
  void Bind();

  T *Pointer();
  void PointerRelease();

  void Data(std::vector<T> &data, unsigned int at);
  void Data(std::vector<T> &data);

  inline unsigned int GetID() const { return ID; }
};

template<class T>
VertexBuffer<T>::VertexBuffer(GLenum _usage)
	: ID(0), size(0), usage(_usage) {
  GLCheck(glGenBuffers(1, &ID));
}

template<class T>
VertexBuffer<T>::~VertexBuffer() {
  GLCheck(glDeleteBuffers(1, &ID));
}

template<class T>
void VertexBuffer<T>::Init(const unsigned long &_size) {
  GL_LOG_TRACE("VertexBuffer<T>::Init (null): Size: {}", _size);
  size = _size;
  GLCheck(glBindBuffer(GL_ARRAY_BUFFER, ID));
  GLCheck(glBufferData(GL_ARRAY_BUFFER, size, NULL, usage));
}

template<class T>
void VertexBuffer<T>::Init(std::vector<T> &data) {
  GL_LOG_TRACE("VertexBuffer<T>::Init (data): Size: {} ({})", data.size(), sizeof(T) * data.size());
  size = sizeof(T) * data.size();
  GLCheck(glBindBuffer(GL_ARRAY_BUFFER, ID));
  GLCheck(glBufferData(GL_ARRAY_BUFFER, size, data.data(), usage));
}

template<class T>
void VertexBuffer<T>::Bind() {
  GLCheck(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

template<class T>
T *VertexBuffer<T>::Pointer() {
  return (T *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

template<class T>
void VertexBuffer<T>::PointerRelease() {
  GLCheck(glUnmapBuffer(GL_ARRAY_BUFFER));
}

template<class T>
void VertexBuffer<T>::Data(std::vector<T> &data, unsigned int at) {
  GLCheck(glBindBuffer(GL_ARRAY_BUFFER, ID));
  auto ptr = Pointer() + at;
  memcpy(ptr, data.data(), sizeof(T) * data.size());
  PointerRelease();
  ptr = nullptr;
}

template<class T>
void VertexBuffer<T>::Data(std::vector<T> &data) {
  size = sizeof(T) * data.size();
  GLCheck(glBindBuffer(GL_ARRAY_BUFFER, ID));
  GLCheck(glBufferData(GL_ARRAY_BUFFER, size, data.data(), usage));
}

} // namespace GL

#endif /* VERTEXBUFFER_H */

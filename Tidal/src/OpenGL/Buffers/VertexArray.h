#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"
#include "VBOLayout.h"

namespace GL {

class VertexArray {
 public:
  unsigned int setAttributes = 0;

  VertexArray();
  virtual ~VertexArray();

  void Init();
  void Bind();
  void SetLayout(VBOLayout vbol);

 private:
  unsigned int ID;
};

} // namespace GL

#endif /* VERTEXARRAY_H */

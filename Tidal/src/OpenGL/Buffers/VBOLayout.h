#ifndef VBOLAYOUT_H
#define VBOLAYOUT_H

namespace GL {
struct VBElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int getSizeOfType(unsigned int type) {
	switch (type) {
	  case GL_FLOAT:return 4;
	  case GL_UNSIGNED_INT:return 4;
	  case GL_UNSIGNED_BYTE:return 1;
	}
	return 0;
  }
};

class VBOLayout {
 private:
  std::vector<VBElement> elements;
  unsigned int stride;

 public:
  VBOLayout()
	  : stride(0) {}

  template<typename T>
  void push(unsigned int count, unsigned char normalized) {
	GL_LOG_WARN("VBOLayout::push() -> Data type unsupported.");
  }

  inline const std::vector<VBElement> getElements() const { return elements; }
  inline unsigned int getStride() const { return stride; }
};

template<>
inline void VBOLayout::push<float>(unsigned int count, unsigned char normalized) {
  elements.push_back({GL_FLOAT, count, normalized});
  stride += count * VBElement::getSizeOfType(GL_FLOAT);
}

template<>
inline void VBOLayout::push<unsigned int>(unsigned int count, unsigned char normalized) {
  elements.push_back({GL_UNSIGNED_INT, count, normalized});
  stride += count * VBElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<>
inline void VBOLayout::push<unsigned char>(unsigned int count, unsigned char normalized) {
  elements.push_back({GL_UNSIGNED_BYTE, count, normalized});
  stride += count * VBElement::getSizeOfType(GL_UNSIGNED_BYTE);
}

} // namespace GL

#endif /* VBOLAYOUT_H */

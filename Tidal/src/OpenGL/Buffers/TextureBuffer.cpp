#include "PCHeader.h"
#include "TextureBuffer.h"

namespace GL {
TextureBuffer::TextureBuffer(GLenum _textureUnit, GLenum _format)
	: textureUnit(_textureUnit), format(_format) {
  PROFILE("TextureBuffer::TextureBuffer");

  GLCheck(glGenBuffers(1, &bufferID));
  GLCheck(glBindBuffer(GL_TEXTURE_BUFFER, bufferID));
  GLCheck(glBufferData(GL_TEXTURE_BUFFER, 1, NULL, GL_DYNAMIC_DRAW));
  GLCheck(glGenTextures(1, &bufferTextureID));
  GLCheck(glBindTexture(GL_TEXTURE_BUFFER, bufferTextureID));
  GLCheck(glTexBuffer(GL_TEXTURE_BUFFER, format, bufferID));
}

TextureBuffer::~TextureBuffer() {
  glDeleteBuffers(1, &bufferID);
}

void TextureBuffer::BindVBO() {
  GLCheck(glBindBuffer(GL_TEXTURE_BUFFER, bufferID));
}

void TextureBuffer::Bind() {
  PROFILE("TextureBuffer::bind");

  GLCheck(glActiveTexture(textureUnit));
  GLCheck(glBindTexture(GL_TEXTURE_BUFFER, bufferTextureID));
  GLCheck(glTexBuffer(GL_TEXTURE_BUFFER, format, bufferID));
}

glm::vec3 *TextureBuffer::Pointer() {
  PROFILE("TextureBuffer::getPointer");

  GLCheck(glBindBuffer(GL_TEXTURE_BUFFER, bufferID));
  return (glm::vec3 *) glMapBuffer(GL_TEXTURE_BUFFER, GL_WRITE_ONLY);
}

void TextureBuffer::PointerRelease() {
  PROFILE("TextureBuffer::releasePointer");

  GLCheck(glUnmapBuffer(GL_TEXTURE_BUFFER));
}
}
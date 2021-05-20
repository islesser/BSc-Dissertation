#include "PCHeader.h"
#include "Program.h"

// Requests deletion of the program through the OpenGL API
GL::Program::~Program() {
  GLCheck(glDeleteProgram(id));
}

// Initializes the program by compiling the shaders, linking them to the program and
// cleaning up before returning.
bool GL::Program::Init(ProgramSetup _setup) {
  config = _setup;

  // VERTEX SHADER
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if (!CompileShader(config.vertex, vertexShader)) {
	return false;
  }

  // GEOMETRY SHADER
  int geometyShader = 0;
  if (config.geometry != NULL) {
	geometyShader = glCreateShader(GL_GEOMETRY_SHADER);
	if (!CompileShader(config.geometry, geometyShader)) {
	  return false;
	}
  }

  // FRAGMENT SHADER
  int fragmentShader = 0;
  if (config.fragment != NULL) {
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!CompileShader(config.fragment, fragmentShader)) {
	  return false;
	}
  }

  id = glCreateProgram();
  GLCheck(glAttachShader(id, vertexShader));

  if (geometyShader != 0) {
	GLCheck(glAttachShader(id, geometyShader));
  }

  if (fragmentShader != 0) {
	GLCheck(glAttachShader(id, fragmentShader));
  }

  // VARYINGS (TRANSFORM FEEDBACK)
  if (config.varyings.size() > 0) {
	glTransformFeedbackVaryings(id, config.varyings.size(), config.varyings.data(), GL_INTERLEAVED_ATTRIBS);
  }

  // LINK PROGRAM
  GLCheck(glLinkProgram(id));

  int success = 0;
  char *infolog;
  GLCheck(glGetProgramiv(id, GL_LINK_STATUS, &success));

  if (!success) {
	glGetProgramInfoLog(id, 512, NULL, infolog);
	GL_LOG_CRITICAL("{}", infolog);
	return false;
  }

  // CLEAN UP
  GLCheck(glDeleteShader(vertexShader));

  if (geometyShader != 0) {
	GLCheck(glDeleteShader(geometyShader));
  }

  if (fragmentShader != 0) {
	GLCheck(glDeleteShader(fragmentShader));
  }

  GL_LOG_TRACE("Program created with id: {}", id);

  return true;
}

// Bind the program for use.
void GL::Program::Use() {
  GLCheck(glUseProgram(id));
}

// Gets the uniform location from the cache, or from the API if not stored.
int GL::Program::UniformLocation(const char *name) {
  if (uniform_locations.find(name) != uniform_locations.end()) {
	return uniform_locations[name];
  } else {
	int a = glGetUniformLocation(id, name);
	uniform_locations[name] = a;
	return a;
  }
}

// Compiles a shader from a filepath
bool GL::Program::CompileShader(const char *filePath, int &id) {
  std::ifstream shaderFile(filePath);

  if (shaderFile.fail()) {
	GL_LOG_CRITICAL("[{}]{}: Could not load file {} ({})", __FILE__, __LINE__, filePath, strerror(errno));
	return false;
  }

  std::string fileContents;
  std::string line;

  while (std::getline(shaderFile, line)) {
	fileContents += line + "\n";
  }

  shaderFile.close();
  const char *contentsPtr = fileContents.c_str();
  GLCheck(glShaderSource(id, 1, &contentsPtr, nullptr));
  GLCheck(glCompileShader(id));
  int success = 0;
  GLCheck(glGetShaderiv(id, GL_COMPILE_STATUS, &success));

  if (!success) {
	GLint length = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
	char *message = (char *) alloca(length * sizeof(char));
	glGetShaderInfoLog(id, length, &length, message);
	GL_LOG_CRITICAL("{}", message);
	GLCheck(glDeleteShader(id);) return false;
  }

  return true;
}

// SECTION: Uniform Helper Functions
void GL::Program::Bool(const char *name, bool value) {
  GLCheck(glUniform1i(UniformLocation(name), (int) value);)
}
// ------------------------------------------------------------------------
void GL::Program::Int(const char *name, int value) {
  GLCheck(glUniform1i(UniformLocation(name), value);)
}
// ------------------------------------------------------------------------
void GL::Program::Float(const char *name, float value) {
  GLCheck(glUniform1f(UniformLocation(name), value);)
}
// ------------------------------------------------------------------------
void GL::Program::Double(const char *name, double value) {
  GLCheck(glUniform1d(UniformLocation(name), value);)
}
// ------------------------------------------------------------------------
void GL::Program::Vec2(const char *name, const glm::vec2 &value) {
  GLCheck(glUniform2fv(UniformLocation(name), 1, &value[0]);)
}
void GL::Program::Vec2(const char *name, float x, float y) {
  GLCheck(glUniform2f(UniformLocation(name), x, y);)
}
// ------------------------------------------------------------------------
void GL::Program::Vec3(const char *name, const glm::vec3 &value) {
  GLCheck(glUniform3fv(UniformLocation(name), 1, &value[0]);)
}
void GL::Program::Vec3(const char *name, float x, float y, float z) {
  GLCheck(glUniform3f(UniformLocation(name), x, y, z);)
}
// ------------------------------------------------------------------------
void GL::Program::Vec4(const char *name, const glm::vec4 &value) {
  GLCheck(glUniform4fv(UniformLocation(name), 1, &value[0]);)
}

void GL::Program::Vec4s(const char *name, int size, const glm::vec4 value[]) {
  GLCheck(glUniform4fv(UniformLocation(name), size, glm::value_ptr(value[0]));)
}

void GL::Program::Vec4(const char *name, float x, float y, float z, float w) {
  GLCheck(glUniform4f(UniformLocation(name), x, y, z, w);)
}
// ------------------------------------------------------------------------
void GL::Program::Mat2(const char *name, const glm::mat2 &mat) {
  GLCheck(glUniformMatrix2fv(UniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void GL::Program::Mat3(const char *name, const glm::mat3 &mat) {
  GLCheck(glUniformMatrix3fv(UniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void GL::Program::Mat4(const char *name, const glm::mat4 &mat) {
  GLCheck(glUniformMatrix4fv(UniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

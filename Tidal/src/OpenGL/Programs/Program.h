#ifndef PROGRAM_H
#define PROGRAM_H

namespace GL {

// Structure used to aggregate configuration information for a shader program.
struct ProgramSetup {
  const char *vertex;
  const char *fragment;
  const char *geometry = NULL;
  std::vector<char *> varyings;
};

// A shader program, supporting vertex, fragment and geometry shaders.
// Also supports transform feedback.
class Program {
 protected:
  unsigned int id = 0; // API ID
  ProgramSetup config; // Configuration data
  std::map<const char *, int> uniform_locations; // Uniform locations stored for faster lookup

  virtual bool CompileShader(const char *filePath, int &id);

 public:
  Program() {};
  virtual ~Program();

  virtual bool Init(ProgramSetup setup);

  void Use();

  inline const unsigned int ID() const { return id; }
  inline const ProgramSetup Config() const { return config; }

  // Uniform functions.
  int UniformLocation(const char *name);

  void Bool(const char *name, bool value);
  void Int(const char *name, int value);
  void Float(const char *name, float value);
  void Double(const char *name, double value);
  void Vec2(const char *name, const glm::vec2 &value);
  void Vec2(const char *name, float x, float y);
  void Vec3(const char *name, const glm::vec3 &value);
  void Vec3(const char *name, float x, float y, float z);
  void Vec4(const char *name, const glm::vec4 &value);
  void Vec4s(const char *name, int size, const glm::vec4 value[]);
  void Vec4(const char *name, float x, float y, float z, float w);
  void Mat2(const char *name, const glm::mat2 &mat);
  void Mat3(const char *name, const glm::mat3 &mat);
  void Mat4(const char *name, const glm::mat4 &mat);
};
} // namespace GL

#endif /* PROGRAM_H */

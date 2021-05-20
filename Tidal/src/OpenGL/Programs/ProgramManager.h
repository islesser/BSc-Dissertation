#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#include "Program.h"

namespace GL {

// Used to for readable shader lookup.
enum PROGRAM_TYPE {
  RENDER_DEBUG = 0,
  RENDER_MESH,
  RENDER_TERRAIN,
  RENDER_WATER,
  SIZE,
  ERROR
};

// Manages the creation, access and destruction of shader programs.
class ProgramManager {
 private:
  std::vector<Program *> programs; // Stored Programs
  PROGRAM_TYPE active = PROGRAM_TYPE::ERROR; // Active program

 public:
  ProgramManager() {};
  virtual ~ProgramManager();
  bool Init();
  void Use(PROGRAM_TYPE type);
  Program *Active();
};

} // namespace GL

#endif /* PROGRAMMANAGER_H */

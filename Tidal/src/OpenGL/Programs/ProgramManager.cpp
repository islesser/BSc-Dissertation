#include "PCHeader.h"
#include "ProgramManager.h"

namespace GL {
// Destroys all stored programs.
ProgramManager::~ProgramManager() {
  for (auto p : programs) {
	delete p;
  }
  programs.clear();
}

// Initializes all default programs.
bool ProgramManager::Init() {
  programs.resize(PROGRAM_TYPE::SIZE, nullptr);

  // DEBUG PROGRAM
  programs[PROGRAM_TYPE::RENDER_DEBUG] = new Program();
  if (!programs[PROGRAM_TYPE::RENDER_DEBUG]->Init({
													  PATH("Shaders/Debug/debug.basic.vertex.glsl"),
													  PATH("Shaders/Debug/debug.basic.fragment.glsl")
												  })) {
	GL_LOG_CRITICAL("GL::ProgramManager::Init() -> Unable to init RENDER_DEBUG program.");
	return false;
  }

  // RENDER MESH PROGRAM
  programs[PROGRAM_TYPE::RENDER_MESH] = new Program();
  if (!programs[PROGRAM_TYPE::RENDER_MESH]->Init({
													 PATH("Shaders/Mesh/mesh.vertex.glsl"),
													 PATH("Shaders/Mesh/mesh.fragment.glsl")
												 })) {
	GL_LOG_CRITICAL("GL::ProgramManager::Init() -> Unable to init RENDER_MESH program.");
	return false;
  }

  // RENDER_TERRAIN PROGRAM
  programs[PROGRAM_TYPE::RENDER_TERRAIN] = new Program();
  if (!programs[PROGRAM_TYPE::RENDER_TERRAIN]->Init({
														PATH("Shaders/Mesh/terrain.vertex.glsl"),
														PATH("Shaders/Mesh/terrain.fragment.glsl")
													})) {
	GL_LOG_CRITICAL("GL::ProgramManager::Init() -> Unable to init RENDER_TERRAIN program.");
	return false;
  }

  // RENDER_WATER PROGRAM
  programs[PROGRAM_TYPE::RENDER_WATER] = new Program();
  if (!programs[PROGRAM_TYPE::RENDER_WATER]->Init({
													  PATH("Shaders/Water/water.vertex.glsl"),
													  PATH("Shaders/Water/water.fragment.glsl")
												  })) {
	GL_LOG_CRITICAL("GL::ProgramManager::Init() -> Unable to init RENDER_WATER program.");
	return false;
  }

  return true;
}

// Binds the requested program.
void ProgramManager::Use(PROGRAM_TYPE type) {
  if (!programs[type]) {
	GL_LOG_CRITICAL("Use Program Call: Type {} has not been initialized!", type);
  } else {
	active = type;
	programs[type]->Use();
  }
}

// Returns a pointer to the active, bound program.
Program *ProgramManager::Active() {
  if (active == PROGRAM_TYPE::ERROR) {
	GL_LOG_CRITICAL("GL::ProgramManager::Active() -> No active program");
	return nullptr;
  }

  return programs[active];
}

} // namespace GL

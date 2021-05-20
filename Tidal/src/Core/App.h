#ifndef APP_H
#define APP_H

#include "OpenGL/Programs/ProgramManager.h"
#include "OpenGL/Renderers/Renderer.h"
#include "Core/GUI/GUILayer.h"
#include "OpenGL/Primitives/Cube.h"
#include "Simulation/Pillar.h"
#include "Utility/Random/Perlin.h"
#include "Utility/Profiler.h"
#include "Simulation/Terrain.h"
#include "Simulation/Water.h"
#include "Simulation/SceneHeightMap.h"

struct DisplayBools
{
  bool terrain = true;
  bool water = true;
};

class App
{
 private:
  Window window;

  double lastFrameTime = 0.0f;
  double dt;

  std::shared_ptr<GL::ProgramManager> programs;
  std::shared_ptr<GL::Renderer> renderer;
  std::shared_ptr<Utils::Perlin> perlin;
  std::vector<Object *> objects;

  SceneHeightMap *shm;
  Terrain *terrain;
  Water *water;
  Pillar *fc;
  GUILayer *gui;
  DisplayBools display;

 public:
  static App *instance;
  App();
  ~App();

  bool init();
  void run();

  DisplayBools GetDisplayBools()
  { return display; }
  void SetDisplayBools(DisplayBools to)
  { display = to; }

};

#endif /* APP_H */

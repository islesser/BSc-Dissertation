#include "PCHeader.h"
#include "App.h"

App *App::instance = nullptr;

App::App()
	: dt(0.0)
{
  App::instance = this;
}

App::~App()
{
  delete gui;
}

bool App::init()
{
  Log::init();

  // Window
  if (!window.Init(WindowConfig(1366, 768, false, false)))
  {
	CORE_LOG_TRACE("EXIT: Window initialization failed");
	return false;
  }
  window.AttachCamera(Camera());
  window.SetupInputHandler();
  // TODO: WHAT IS THIS???? window.AttachMouseSelector(window.GetCamera());

  // GUI
  gui = new GUILayer();
  if (!gui->init(window.Context()))
  {
	CORE_LOG_TRACE("EXIT: GUI Layer initialization failed");
	return false;
  }

  SceneBounds scenebounds = {
	  {0.0f, 0.0f, 0.0f},
	  {400.0f, 150.0f, 400.0f}
  };


  float resolution = 1;
  float waterlevel = 50.0f;

  shm = new SceneHeightMap();

  perlin = std::make_shared<Utils::Perlin>();

  terrain = new Terrain(gui, scenebounds.origin.x, scenebounds.origin.y, scenebounds.origin.z, scenebounds.size.x, scenebounds.size.y, scenebounds.size.z, perlin, resolution);
  terrain->Generate();
  shm->SetTerrain(terrain->HeightMap());

  water = new Water(gui, scenebounds.origin.x, waterlevel, scenebounds.origin.z, scenebounds.size.x, scenebounds.size.y, scenebounds.size.z * 0.75f, resolution);
  water->Generate();

  // Programs
  programs = std::make_shared<GL::ProgramManager>();
  if (!programs->Init())
  {
	GL_LOG_CRITICAL("GL::Renderer::init() -> Program Manager failed to init");
	return false;
  }

  // Renderers
  renderer = std::make_shared<GL::Renderer>();
  if (!renderer->Init(gui, programs))
  {
	CORE_LOG_TRACE("EXIT: Renderer initialization failed");
	return false;
  }

  fc = new Pillar(scenebounds.size.x * 0.5f - 20.0f, 0.0f, scenebounds.size.z * 0.75f * 0.25f, 10.0f, 70.0f, 10.0f);
  fc->SetSceneBounds(scenebounds);
  fc->Generate();
  objects.push_back(fc);

  shm->Init(objects, waterlevel);

  return true;
}

void App::run()
{
  float waterlevel = 50.0f;
  while (window.IsActive())
  {
	double time = glfwGetTime() / 1000.0f;
	dt = (time - lastFrameTime);
	lastFrameTime = time;

	auto IH = window.GetInputHandler();
	IH->Update();

	glfwPollEvents();
	processInput(&window);

	if(fc->Update(&window)) {
		shm->RefreshSceneHeightMap(objects, waterlevel);
	}

	//water->Update(terrain->HeightMap()->heights);

	// Begin Draw
	window.Clear();
	shm->Bind();

	if (display.terrain)
	  renderer->Draw(dt, terrain);
	if (display.water)
	  renderer->Draw(dt, water);
	renderer->Draw(dt, fc);

	if (IH->Mode() == CAMERA) renderer->Draw(dt, IH->GetWebcamCursor());

	gui->begin();
	gui->constantElements();
	gui->render();
	gui->end();

	window.SwapBuffers();
	// End Draw
  }
}
#include "PCHeader.h"
#include "GUILayer.h"

#include "../App.h"

// Shuts down ImGUI and it's context.
GUILayer::~GUILayer() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

// Configure and initializes the ImGUI context.
bool GUILayer::init(GLFWwindow *_window) {
  window = _window;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();
  (void) io;
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
  io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

  ImGui::StyleColorsDark();

  ImGuiStyle &style = ImGui::GetStyle();
  if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");

  return true;
}

// Add a display element to the list.
// A display element is a window that is supplied by a segment of the application
// and displays values for that segment.
void GUILayer::addElement(std::function<void()> window) {
  windows.push_back(window);
}

// Renders the interface. Contains a default window which displays FPS and camera
// information, and then iterates through the display windows and renders those.
void GUILayer::render() {
  Window::instance->SetMouseOverGUI(ImGui::IsWindowHovered(ImGuiFocusedFlags_AnyWindow));

  {
	ImGui::Begin("Debug Variables");
	{
	  // Framerate / time
	  ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	  auto debug = App::instance->GetDisplayBools();
	  ImGui::Checkbox("Draw Terrain", &debug.terrain);
	  ImGui::Checkbox("Draw Water", &debug.water);
	  App::instance->SetDisplayBools(debug);

	  auto c = Window::instance->GetCamera();

	  auto f = c->Front();
	  auto u = c->Up();
	  auto p = c->Position();
	  auto yp = c->YP();

	  ImGui::Text("Camera");
	  ImGui::Text("Front: %.3f, %.3f, %.3f", f.x, f.y, f.z);
	  ImGui::Text("Up: %.3f, %.3f, %.3f", u.x, u.y, u.z);
	  ImGui::Text("Position: %.3f, %.3f, %.3f", p.x, p.y, p.z);
	  ImGui::Text("Yaw / Pitch: %.3f, %.3f", yp.x, yp.y);
	}
	ImGui::End();
  }

  for (auto w : windows) {
	{
	  w();
	}
  }
}

// Starts the ImGUI frame / render process.
void GUILayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

// Ends the ImGUI frame / render process.
void GUILayer::end() {
  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
	GLFWwindow *backup_current_context = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(backup_current_context);
  }
}

// Set up and display constant elements, in this case the docking system of
// ImGUI and menu bar.
void GUILayer::constantElements() {

  static bool opt_fullscreen_persistant = true;
  bool opt_fullscreen = opt_fullscreen_persistant;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (opt_fullscreen) {
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |=
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }

  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	window_flags |= ImGuiWindowFlags_NoBackground;

  bool a = true;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace Demo", &a, window_flags);
  ImGui::PopStyleVar();

  if (opt_fullscreen)
	ImGui::PopStyleVar(2);

  if (io->ConfigFlags & ImGuiConfigFlags_DockingEnable) {
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  if (ImGui::BeginMenuBar()) {
	if (ImGui::BeginMenu("Docking")) {
	  if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
		dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
	  if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
		dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
	  if (ImGui::MenuItem("Flag: NoDockingInCentralNode",
						  "",
						  (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
		dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
	  if (ImGui::MenuItem("Flag: PassthruCentralNode",
						  "",
						  (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))
		dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
	  if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
		dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
	  ImGui::Separator();
	  ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
  }
  ImGui::End();
}

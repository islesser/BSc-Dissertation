#ifndef WINDOW_H
#define WINDOW_H

#include "Camera.h"
#include "MouseSelector.h"
#include "Core/Window/InputHandler.h"

class Window;

// Callback forward declarations.
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *m, int button, int action, int mods);
void processInput(Window *window);
void glfw_error_callback(int error, const char *description);

// Window configuration object struct.
struct WindowConfig {
  unsigned int width = 1366; // Window width
  unsigned int height = 768; // Window height
  bool wireframe = false; // Wireframe mode on/off
  bool fullscreen = false; // Fullscreen mode on/off
  const char *title = "EEUB53"; // Window title.

  WindowConfig() {}
  WindowConfig(unsigned int w, unsigned int h, bool wf, bool fs) : width(w), height(h), wireframe(wf), fullscreen(fs) {}
  WindowConfig(unsigned int w, unsigned int h, const char *t) : width(w), height(h), title(t) {}
};

// Display window, controls display related functions and manages the camera.
class Window {
 private:
  struct WindowConfig config; // Configuration data
  GLFWwindow *ctx; // GLFW context
  Camera camera; // Managed camera
  InputHandler *ih; // Input handler
  bool mouseHeldLeft = false; // Is mouse left button held?
  bool mouseHeldRight = false; // Is mouse right button held?
  bool mouseOverGUI = false; // Is the mouse over the GUI?

 public:
  static Window *instance; // Accessor instance
  glm::vec2 MousePosition; // Stored mouse position
  glm::vec2 MouseOffsets; // Stored mouse offsets

  // Constructs the window
  Window() {
	Window::instance = this;
	DefaultConfig();
  }

  // Tear-downs the window
  ~Window() {
	glfwDestroyWindow(ctx);
	glfwTerminate();
  }

  bool Init(const WindowConfig _config = {});
  void Clear();
  void SwapBuffers();
  void AttachCamera(Camera _camera);

  // Inline getters / setters.
  void SetupInputHandler() { ih = new InputHandler(); }
  inline bool IsActive() { return !glfwWindowShouldClose(ctx); }
  inline void DefaultConfig() { config = {}; }
  inline GLFWwindow *Context() const { return ctx; }
  inline unsigned int GetWidth() const { return config.width; }
  inline unsigned int GetHeight() const { return config.height; }
  inline float AspectRatio() const { return (float) config.width / (float) config.height; }
  inline Camera *GetCamera() { return &camera; }
  inline InputHandler *GetInputHandler() { return ih; }
  inline void SetMousePosition(glm::vec2 to) { MousePosition = to; }
  inline void SetLeftMouseHeld(bool to) { mouseHeldLeft = to; }
  inline bool IsLeftMouseHeld() const { return mouseHeldLeft; }
  inline void SetRightMouseHeld(bool to) { mouseHeldRight = to; }
  inline bool IsRightMouseHeld() const { return mouseHeldRight; }
  inline void SetMouseOverGUI(bool to) { mouseOverGUI = to; }
  inline bool IsMouseOverGUI() const { return mouseOverGUI; }
};

#endif /* WINDOW_H */

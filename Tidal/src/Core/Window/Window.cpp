#include "PCHeader.h"
#include "Window.h"

#include "Core/Window/InputCallbacks.h"

Window *Window::instance = nullptr;

// Initializes the window, setting up the window itself and also the
// opengl configuration.
bool Window::Init(const WindowConfig _config)
{
  	config = _config;
    CORE_LOG_INFO("Starting GLFW: {}", glfwGetVersionString());
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        CORE_LOG_CRITICAL("Window: could not start GLFW");
        return false;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    ctx = glfwCreateWindow(config.width, config.height, config.title, config.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

    if (ctx == NULL)
    {
        CORE_LOG_CRITICAL("Window: GLFWwindow init failure");
        return false;
    }

    glfwMakeContextCurrent(ctx);
	glfwSetFramebufferSizeCallback(ctx, framebuffer_size_callback);
	glfwSetCursorPosCallback(ctx, mouse_callback);
	glfwSetMouseButtonCallback(ctx, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        CORE_LOG_CRITICAL("Window: GLAD init failure");
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);

    return true;
}

// Clear the framebuffer to a certain colour.
void Window::Clear()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Attaches a camera to the window
void Window::AttachCamera(Camera _camera)
{
  camera = _camera;
}

// Swap the buffers
void Window::SwapBuffers()
{
  glfwSwapBuffers(ctx);
}


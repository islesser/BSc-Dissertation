#ifndef TIDAL_TIDAL_SRC_CORE_WINDOW_INPUTCALLBACKS_H_
#define TIDAL_TIDAL_SRC_CORE_WINDOW_INPUTCALLBACKS_H_

// Process the input each frame. Checks the key states and issues the commands to
// the other areas of the application.
void processInput(Window *window)
{
  InputHandler *IH = Window::instance->GetInputHandler();
  Camera *camera = window->GetCamera();
  auto CTX = window->Context();

  if (glfwGetKey(CTX, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(CTX, true);

  if (glfwGetKey(CTX, GLFW_KEY_W) == GLFW_PRESS)
	camera->ChangePosition(camera->Front());
  if (glfwGetKey(CTX, GLFW_KEY_S) == GLFW_PRESS)
	camera->ChangePosition(-camera->Front());

  if (glfwGetKey(CTX, GLFW_KEY_Q) == GLFW_PRESS)
	camera->ChangePosition(camera->Up());
  if (glfwGetKey(CTX, GLFW_KEY_E) == GLFW_PRESS)
	camera->ChangePosition(-camera->Up());

  if (glfwGetKey(CTX, GLFW_KEY_A) == GLFW_PRESS)
	camera->ChangePosition(-glm::normalize(glm::cross(camera->Front(), camera->Up())));
  if (glfwGetKey(CTX, GLFW_KEY_D) == GLFW_PRESS)
	camera->ChangePosition(glm::normalize(glm::cross(camera->Front(), camera->Up())));

  if (glfwGetKey(CTX, GLFW_KEY_1) == GLFW_PRESS)
	camera->SetTo3D();
  if (glfwGetKey(CTX, GLFW_KEY_2) == GLFW_PRESS)
	camera->SetToTopdown();

  if (glfwGetKey(CTX, GLFW_KEY_O) == GLFW_PRESS)
	IH->SetMode(INPUT_MODE::MOUSE);
  if (glfwGetKey(CTX, GLFW_KEY_P) == GLFW_PRESS)
	IH->SetMode(INPUT_MODE::CAMERA);
}

// Called by the application whenever the mouse is moved.
// Calculates the offset from the previous mouse position and
// saves the mouse state.
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  Window::instance->SetMousePosition(glm::vec2(xpos, ypos));

  static float prevX = 0.0f;
  static float prevY = 0.0f;

  static bool firstRMouse = true;
  if (Window::instance->IsRightMouseHeld())
  {
	if (firstRMouse)
	{
	  prevX = xpos;
	  prevY = ypos;
	  firstRMouse = false;
	}

	float xoffset = xpos - prevX;
	float yoffset = prevY - ypos;

	prevX = xpos;
	prevY = ypos;

	Window::instance->GetCamera()->YawPitch(xoffset, yoffset);
  } else
  {
	firstRMouse = true;
  }

  static bool firstLMouse = true;
  if (Window::instance->IsLeftMouseHeld())
  {
	if (firstLMouse)
	{
	  prevX = xpos;
	  prevY = ypos;
	  firstLMouse = false;
	}

	float xoffset = xpos - prevX;
	float yoffset = prevY - ypos;

	Window::instance->MouseOffsets = glm::vec2(xoffset, yoffset);

	prevX = xpos;
	prevY = ypos;
  }
  else
  {
	Window::instance->MouseOffsets = glm::vec2(0.0f);
	firstLMouse = true;
  }
}

// Called when a mouse button is pressed.
void mouse_button_callback(GLFWwindow *m, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
	if (action == GLFW_PRESS)
	  Window::instance->SetLeftMouseHeld(true);
	else if (action == GLFW_RELEASE)
	  Window::instance->SetLeftMouseHeld(false);
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT)
  {
	if (action == GLFW_PRESS)
	  Window::instance->SetRightMouseHeld(true);
	else if (action == GLFW_RELEASE)
	  Window::instance->SetRightMouseHeld(false);
  }
}

// Called when the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// Called when an error occurs
void glfw_error_callback(int error, const char *description)
{
  CORE_LOG_CRITICAL("GLFW ERROR: code {} msg: {}", error, description);
}

#endif //TIDAL_TIDAL_SRC_CORE_WINDOW_INPUTCALLBACKS_H_

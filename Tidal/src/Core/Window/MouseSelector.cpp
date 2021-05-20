#include "PCHeader.h"
#include "MouseSelector.h"
#include "Window.h"

// Updates the mouse coordinates converting the mouse position to
// world coordinates.
void MouseSelector::Update(float mouseX, float mouseY)
{
  glm::vec2 NDC = ToNDC(mouseX, mouseY);
  glm::vec4 Clip = glm::vec4(NDC.x, NDC.y, -1.0f, 1.0f);
  glm::vec4 Eye = ToEye(Clip);
  ray = ToWorld(Eye);
}

// Updates from an object position, used for the web cam input.
void MouseSelector::Update(glm::ivec2 objectPosition)
{
  glm::vec2 NDC = ToNDC(objectPosition.x, objectPosition.y);
  glm::vec4 Clip = glm::vec4(NDC.x, NDC.y, -1.0f, 1.0f);
  glm::vec4 Eye = ToEye(Clip);
  ray = ToWorld(Eye);
}

// Converts from eye space to world space
glm::vec3 MouseSelector::ToWorld(glm::vec4 eye) {
  glm::mat4 invertedView = glm::inverse(camera->View());
  glm::vec4 rayWorld = invertedView * eye;
  glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
  glm::normalize(mouseRay);
  return mouseRay;
}

// Converts from clip space to eye space
glm::vec4 MouseSelector::ToEye(glm::vec4 clip) {
  glm::mat4 invertedProjection = glm::inverse(camera->Projection(Window::instance->AspectRatio()));
  glm::vec4 eyeCoords = invertedProjection * clip;
  return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

// Converts the mouse position to normalized device co-ordinates.
glm::vec2 MouseSelector::ToNDC(float mouseX, float mouseY) {
  auto display = glfwGetVideoMode(glfwGetPrimaryMonitor());
  float x = (2.0f * mouseX) / Window::instance->GetWidth() - 1.0f;
  float y = 1.0f - (2.0f * mouseY) / Window::instance->GetHeight();
  return glm::vec2(x, y);
}

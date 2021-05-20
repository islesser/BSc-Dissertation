#include "PCHeader.h"
#include "Camera.h"

// Update the camera, adjusting front, right and up values.
void Camera::Update()
{
  glm::vec3 _front;
  _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  _front.y = sin(glm::radians(pitch));
  _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(_front);

  right = glm::normalize(glm::cross(front, worldUp));
  up    = glm::normalize(glm::cross(right, front));
}

// Changes the position by the passed value (scalar)
void Camera::ChangePosition(float by)
{
  position += by * speed;
}

// Changes the position by the passed value (vector)
void Camera::ChangePosition(glm::vec3 by)
{
  position += by * speed;
}

// Changes the yaw and pitch, calling Update() when done.
void Camera::YawPitch(float _yaw, float _pitch)
{
  _yaw *= sensitivity;
  _pitch *= sensitivity;

  yaw   += _yaw;
  pitch += _pitch;

  if (pitch > 89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;

  Update();
}

// Set the camera to the top down view preset.
void Camera::SetToTopdown()
{
  position = glm::vec3(400.0f, 840.0f, 300.0f);
  yaw = 90.0f;
  pitch = -89.0f;
  Update();
}

// Set the camera to the 3D perspective preset.
void Camera::SetTo3D()
{
  position = glm::vec3(-100.0f, 100.0f,  -90.0f);
  yaw = 45.0f;
  pitch = 0.0f;
  Update();
}

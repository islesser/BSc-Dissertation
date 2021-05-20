#ifndef PARTICLE_PARTICLE_SRC_CORE_WINDOW_CAMERA_H_
#define PARTICLE_PARTICLE_SRC_CORE_WINDOW_CAMERA_H_

// Camera class, giving a perspective view of the world.
// Contains functions for getting the view and projection matrix and
// alternating between presets.
class Camera {
 private:
  glm::vec3 position = glm::vec3(-100.0f, 100.0f, -90.0f); // Camera position.
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f); // Camera frontal direction.
  glm::vec3 right; // Camera right direction.
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Camera up direction.
  glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f); // World up direction

  float speed = 1.0f; // Camera move speed.
  float sensitivity = 0.5f; // Camera move sensitivity
  float yaw = 45.0f; // Camera yaw
  float pitch = 0.0f; // Camera pitch

  float fov = 45.0f; // Field of view
  float near = 0.1f; // Near plane
  float far = 10000.0f; // Far plane

 private:
  void Update();

 public:
  Camera() {};
  virtual ~Camera() {};

  void ChangePosition(float by);
  void ChangePosition(glm::vec3 by);
  void YawPitch(float _yaw, float _pitch);

  // Calculates and returns the view matrix.
  inline glm::mat4 View() { return glm::lookAt(position, position + front, up); }
  // Calculates and returns the projection matrix.
  inline glm::mat4 Projection(float aspect) { return glm::perspective(glm::radians(fov), aspect, near, far); }
  // Returns the position
  inline glm::vec3 Position() { return position; }
  // Returns the front
  inline glm::vec3 Front() { return front; }
  // Returns the camera up
  inline glm::vec3 Up() { return up; }
  // Return the yaw and pitch as a vec2
  inline glm::vec2 YP() { return glm::vec2(yaw, pitch); }

  void SetToTopdown();
  void SetTo3D();
};

#endif //PARTICLE_PARTICLE_SRC_CORE_WINDOW_CAMERA_H_

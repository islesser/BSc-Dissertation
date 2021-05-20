#include <ComputerVision/WebcamObjectTracker.h>
#include "PCHeader.h"
#include "Pillar.h"

// Construct the pillar object.
Pillar::Pillar(float x, float y, float z, float width, float height, float depth)
	: FlatCube(x, y, z, width, height, depth) {
  Generate();
  Min = glm::vec3(position.x + dimensions.x * 0.5f,
				  position.y + dimensions.y * 0.5f,
				  position.z + dimensions.z * 0.5f);
  Max = glm::vec3(position.x + dimensions.x + dimensions.x * 0.5f,
				  position.y + dimensions.y + dimensions.y * 0.5f,
				  position.z + dimensions.z + dimensions.z * 0.5f);
  SetTint(glm::vec4(0.0f, 2.0f, 1.0f, 1.0f));
}

// Performs on-frame update for the object.
bool Pillar::Update(Window *window) {

  // Either mouse or camera picking.
  auto InputMode = window->GetInputHandler()->Mode();

  // Check intersections
  bool hasIntersected;
  if (InputMode == MOUSE) hasIntersected = IntersectMouseSelector(window);
  else if (InputMode == CAMERA) hasIntersected = IntersectWebcamCursor(window->GetInputHandler()->GetWebcamCursor());

  if (hasIntersected || selectedLastFrame) {

    // Highlight to show it's selected
	SetTint(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Check if the object should be moved.
	bool shouldMoveObject;
	if (InputMode == MOUSE) shouldMoveObject = Window::instance->IsLeftMouseHeld();
	else if (InputMode == CAMERA)
	  shouldMoveObject = glfwGetKey(Window::instance->Context(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

	if (shouldMoveObject) {
	  selectedLastFrame = true;

	  // Get the movement offsets depending on the input mode.
	  if (InputMode == MOUSE) {
		auto o = Window::instance->MouseOffsets;
		float clampedX = std::clamp(o.x, -1.0f, 1.0f);
		float clampedY = std::clamp(o.y, -1.0f, 1.0f);
		position.x -= clampedX;
		position.z += clampedY;
	  } else if (InputMode == CAMERA) {
		auto moveOffset = WebcamObjectTracker::instance->MappedOffset();
		position.x += moveOffset.x;
		position.z -= moveOffset.y;
	  }

	  // Bounds checking
	  CheckSceneBounds();

	  // Update bounds and create the model matrix
	  modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
		  position.x + dimensions.x * 0.5f,
		  position.y + dimensions.y * 0.5f,
		  position.z + dimensions.z * 0.5f)) * /* Rotate here if needed */
		  glm::scale(glm::mat4(1.0f), dimensions);

	  Min = glm::vec3(position.x + dimensions.x * 0.5f,
					  position.y + dimensions.y * 0.5f,
					  position.z + dimensions.z * 0.5f);
	  Max = glm::vec3(position.x + dimensions.x + dimensions.x * 0.5f,
					  position.y + dimensions.y + dimensions.y * 0.5f,
					  position.z + dimensions.z + dimensions.z * 0.5f);

	  return true;
	} else {
	  selectedLastFrame = false;
	}
  } else {
    // Deselection
	SetTint(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	selectedLastFrame = false;
  }

  return false;
}

// Check intersection from a ray created with the camera picker.
bool Pillar::IntersectWebcamCursor(WebcamCursor *wcc) {
  auto wccorigin = wcc->Position();
  auto wccdims = wcc->Dimensions();

  glm::vec3 point = glm::vec3(
	  wccorigin.x + wccdims.x * 0.5f,
	  wccorigin.y + wccdims.y * 0.5f,
	  wccorigin.z + wccdims.z * 0.5f
  );

  return (point.x >= Min.x && point.x <= Max.x) &&
	  (point.z >= Min.z && point.z <= Max.z);
}

// Check intersection from a ray created with the mouse picker.
bool Pillar::IntersectMouseSelector(Window *window) {
  auto origin = window->GetCamera()->Position();
  auto ray = window->GetInputHandler()->GetMouseSelector()->Ray();

  float tMin = (Min.x - origin.x) / ray.x;
  float tMax = (Max.x - origin.x) / ray.x;

  if (tMin > tMax) std::swap(tMin, tMax);

  float tyMin = (Min.y - origin.y) / ray.y;
  float tyMax = (Max.y - origin.y) / ray.y;

  if (tyMin > tyMax) std::swap(tyMin, tyMax);

  if ((tMin > tyMax) || (tyMin > tMax))
	return false;

  if (tyMin > tMin)
	tMin = tyMin;

  if (tyMax < tMax)
	tMax = tyMax;

  float tzMin = (Min.z - origin.z) / ray.z;
  float tzMax = (Max.z - origin.z) / ray.z;

  if (tzMin > tzMax) std::swap(tzMin, tzMax);

  if ((tMin > tzMax) || (tzMin > tMax))
	return false;

  return true;
}

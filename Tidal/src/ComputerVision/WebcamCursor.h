#ifndef TIDAL_TIDAL_SRC_COMPUTERVISION_WEBCAMCURSOR_H_
#define TIDAL_TIDAL_SRC_COMPUTERVISION_WEBCAMCURSOR_H_

#include "OpenGL/Primitives/Cube.h"
#include "ComputerVision/WebcamObjectTracker.h"

class Window;

// A cursor for use with the webcam input method, this is displayed as a
// small cube who's position relates to the calculated ray position.
// Derived class of the FlatCube class.
class WebcamCursor : public FlatCube
{
public:
  WebcamCursor()
  	: FlatCube(0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f)
  {
	Generate(); // Generate vertex / index data.
  }

  // Update the position of the WebcamCursor.
  // Calculates position, checks bounds and then creates a model matrix.
  void Update(Window *window) {

    // Get the camera position.
    auto cameraposition = WebcamObjectTracker::instance->Position();

	// Maps the camera position to screen size.
    auto cameramapped = WebcamObjectTracker::instance->MappedPosition(cameraposition);

    // Construct th position.
    position = glm::vec3(cameramapped.x, 50.0f, cameramapped.y);

    // Ensure we're not going out of bounds.
    CheckSceneBounds();

    // Create the model matrix from translation and scale.
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
		position.x + dimensions.x * 0.5f,
		position.y + dimensions.y * 0.5f,
		position.z + dimensions.z * 0.5f)) * /*NO ROTATION */
		glm::scale(glm::mat4(1.0f), dimensions);
  }

};

#endif //TIDAL_TIDAL_SRC_COMPUTERVISION_WEBCAMCURSOR_H_

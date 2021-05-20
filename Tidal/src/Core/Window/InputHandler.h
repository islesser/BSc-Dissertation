#ifndef TIDAL_TIDAL_SRC_CORE_WINDOW_INPUTHANDLER_H_
#define TIDAL_TIDAL_SRC_CORE_WINDOW_INPUTHANDLER_H_

#include "ComputerVision/WebcamCursor.h"
#include "ComputerVision/WebcamObjectTracker.h"
#include "Core/Window/MouseSelector.h"

// Input modes, either mouse or the camera.
enum INPUT_MODE {
  MOUSE = 0,
  CAMERA
};

// Manages the input aspect of the application, setting up
// input callbacks and polling functions.
class InputHandler {
 public:
  InputHandler();
  virtual ~InputHandler();

  void SetMode(INPUT_MODE to);
  void ChangeMode();
  void Update();

  // Return the input mode.
  inline INPUT_MODE Mode() { return mode; }
  // Return the mouse selector object
  inline MouseSelector *GetMouseSelector() { return ms; }
  // Return the webcam cursor object
  inline WebcamCursor *GetWebcamCursor() { return wcc; }

 private:
  INPUT_MODE mode = INPUT_MODE::MOUSE; // Input mode.
  MouseSelector *ms; // Mouse selector (mouse picker)
  WebcamObjectTracker *wcoj = nullptr; // Web cam object selector
  WebcamCursor *wcc = nullptr; // Cursor for the web cam object§
};

#endif //TIDAL_TIDAL_SRC_CORE_WINDOW_INPUTHANDLER_H_

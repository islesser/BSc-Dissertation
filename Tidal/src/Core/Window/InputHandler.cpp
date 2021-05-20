#include "PCHeader.h"
#include "InputHandler.h"
#include "Core/Window/Window.h"

// Sets up the mouse selector by default.
InputHandler::InputHandler() {
  ms = new MouseSelector(Window::instance->GetCamera());
}

// Tears-down the created objects.
InputHandler::~InputHandler() {
  if (ms != nullptr) delete ms;
  if (ms != nullptr) delete wcoj;
  if (ms != nullptr) delete wcc;
}

// Set the input mode, checking to ensure the requested mode
// is not already active.
void InputHandler::SetMode(INPUT_MODE to) {
  if (to == mode) return; // Do nothing.

  mode = to;
  ChangeMode();
}

// Changes the input mode.
// If it is to change to web cam mode, it will create and
// initialize these objects, else destroys them.
// They are destroyed as the web cam input mode is resource intensive
void InputHandler::ChangeMode() {
  switch (mode) {
	case MOUSE: {
	  // Destroy the web cam related objects.
	  delete wcoj;
	  wcoj = nullptr;
	  delete wcc;
	  wcc = nullptr;
	  break;
	}
	case CAMERA: {
	  // Setup the web cam objects.
	  wcoj = new WebcamObjectTracker();
	  wcc = new WebcamCursor();
	  SceneBounds scenebounds = {
		  {0.0f, 0.0f, 0.0f},
		  {400.0f, 150.0f, 400.0f}
	  };
	  wcc->SetSceneBounds(scenebounds);
	  break;
	}
  };
}

// Update the input objects depending on the selected mode.
void InputHandler::Update() {
  switch (mode) {
	case MOUSE: {
	  auto mousePos = Window::instance->MousePosition;
	  ms->Update(mousePos.x, mousePos.y);
	  break;
	}
	case CAMERA: {
	  wcoj->Update();
	  ms->Update(wcoj->Position());
	  wcc->Update(Window::instance);
	  break;
	}
  };
}
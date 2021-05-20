#include "PCHeader.h"
#include "Renderer.h"

namespace GL {
bool Renderer::Init(GUILayer *gui, std::shared_ptr<ProgramManager> _programs) {
  PROFILE("Renderer::init");
  programs = _programs;
  gui->addElement([this]() {
	this->GUIElement("Renderer");
  });
  return true;
}

// Handles shader uniform variable setup and then performs the draw call from a passed
// Renderable object (ie. 1 object = 1 draw call)
void Renderer::Draw(double dt, Renderable *r) {

  // Set shader variables
  programs->Use(r->Program());
  Camera *camera = Window::instance->GetCamera();
  programs->Active()->Float("dt", (float) dt);
  programs->Active()->Mat4("view", camera->View());
  programs->Active()->Mat4("projection", camera->Projection(Window::instance->AspectRatio()));
  programs->Active()->Vec3("LightPosition", LightPosition);
  programs->Active()->Vec3("cameraPosition", camera->Position());
  programs->Active()->Float("AmbientStrength", AmbientStrength);
  programs->Active()->Float("SpecularStrength", SpecularStrength);
  programs->Active()->Vec4("Tint", r->Tint());

  r->Begin(programs->Active());
  auto model = r->Model();
  programs->Active()->Mat4("model", r->Model());

  // Performs either a standard or indexed draw depending on the layout of the Renderable.
  if (r->Indices() != 0) {
	GLCheck(glDrawElements(r->Mode(), r->Indices(), GL_UNSIGNED_INT, nullptr));
  } else {
	GLCheck(glDrawArrays(r->Mode(), 0, r->Vertices()));
  }
}

// Handles shader uniform variable setup and then performs the draw call from a passed
// Datastore object (ie. 1 datastore = multiple models = 1 draw call)
void Renderer::Draw(double dt, std::shared_ptr<Datastore> datastore) {
  programs->Use(datastore->Program());
  datastore->Begin(programs->Active());

  Camera *camera = Window::instance->GetCamera();
  programs->Active()->Float("dt", (float) dt);
  programs->Active()->Mat4("model", glm::mat4(1.0f));
  programs->Active()->Mat4("view", camera->View());
  programs->Active()->Mat4("projection", camera->Projection(Window::instance->AspectRatio()));
  programs->Active()->Float("AmbientStrength", AmbientStrength);
  programs->Active()->Float("SpecularStrength", SpecularStrength);
  GLCheck(glDrawArrays(datastore->Mode(), 0, datastore->Size()));
}

// GUI Element to allow interactivity of contained values.
void Renderer::GUIElement(std::string name) {
  ImGui::Begin(name.c_str());
  {
	ImGui::DragFloat("AmbientStrength", &AmbientStrength, 0.1f, 0.0f, 1.0f, "%.2f");
	ImGui::DragFloat("SpecularStrength", &SpecularStrength, 0.01f, 0.0f, 1.0f, "%.2f");
	ImGui::SliderFloat3("LightPosition", &LightPosition.x, -1000.0f, 1000.0f);
  }
  ImGui::End();
}

}
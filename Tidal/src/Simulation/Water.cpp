#include "PCHeader.h"
#include "Water.h"

// Constructor
Water::Water(GUILayer *gui, float x, float y, float z, float w, float h, float d, float resolution)
	: Plane(x, y, z, w, h, d, resolution) {
  // Set the render-shader
  Set(GL::RENDER_WATER, GL_TRIANGLES);

  // Add GUI element to GUI layer
  gui->addElement([this]() {
	this->GUIElement("Water Plane");
  });
}

void Water::Generate() {

  // Get number of cells in each direction
  int cellsX = dimensions.x / resolution;
  int cellsZ = dimensions.z / resolution;

  // Create Vertices
  glm::vec4 COLOUR = glm::vec4(0.21f, 0.55f, 0.77f, 1.0f);
  for (float z = 0; z < cellsZ; ++z) {
	for (float x = 0; x < cellsX; ++x) {
	  int xp = position.x + x * resolution;
	  int zp = position.z + z * resolution;

	  Vertex vertex = {{xp, position.y, zp}, COLOUR, {0.0f, 0.0f, 0.0f}};

	  vertices.push_back(vertex);
	  vertices2.push_back(vertex); // CPU impl test
	}
  }

  // Calculate indices
  for (int z = 0; z < cellsZ - 1; ++z) {
	int Z = z * (cellsX); // Row
	int ZN = (z + 1) * (cellsX); // Next Row

	for (int x = 0; x < cellsX - 1; ++x) {
	  indices.push_back(Z + x); // A
	  indices.push_back(ZN + x + 1); // C
	  indices.push_back(ZN + x); // D
	  indices.push_back(ZN + x + 1); // C
	  indices.push_back(Z + x); // A
	  indices.push_back(Z + x + 1); // B
	}
  }

  // Sets the data in the buffers.
  __Init(vertices, indices);
}

// Set the program uniforms and bind buffers.
void Water::Begin(GL::Program *program) {
  Bind();
  program->Float("time", (float) glfwGetTime());

  program->Int("THM", 0);
  program->Int("HM", 1);
  program->Int("DM", 2);
  program->Float("waterlevel", dimensions.y);
  program->Float("foamHeight", foamHeight);
  program->Float("shorethreshold", shorethreshold);
  program->Float("min", Min);
  program->Float("max", Max);
  program->Int("sceneWidth", dimensions.x);

  program->Vec4("wave0", wave0);
  program->Vec4("wave1", wave1);
  program->Vec4("wave2", wave2);

  program->Vec4("OceanColour", OceanColour);
}

// Items to be displayed on the GUI to allow real-time control of the element.
void Water::GUIElement(std::string name) {
  ImGui::Begin(name.c_str());
  {
	ImGui::DragFloat("Min", &Min, 0.01f, 0.0f, 1.0f, "%.2f");
	ImGui::DragFloat("Max", &Max, 0.01f, 0.0f, 10.0f, "%.2f");

	ImGui::Text("General");
	ImGui::DragFloat("Foam Height", &foamHeight, 0.001f, 50.0f, 55.0f, "%.3f");
	ImGui::DragFloat("Shore Threshold", &shorethreshold, 0.01f, -10.0f, 50.0f, "%.2f");
	ImGui::ColorEdit4("Ocean Colour", &OceanColour.x);

	ImGui::Text("Wave 0");
	ImGui::DragFloat2("Direction 0", &wave0.x, 0.01f, -1.0f, 1.0f, "%.2f");
	ImGui::DragFloat("Wavelength 0", &wave0.w, 0.01f, 0.0f, 100.0f, "%.2f");
	ImGui::DragFloat("Steepness 0", &wave0.z, 0.001f, 0.0f, 1.0f, "%.3f");

	ImGui::Text("Wave 1");
	ImGui::DragFloat2("Direction 1", &wave1.x, 0.01f, -1.0f, 1.0f, "%.2f");
	ImGui::DragFloat("Wavelength 1", &wave1.w, 0.01f, 0.0f, 100.0f, "%.2f");
	ImGui::DragFloat("Steepness 1", &wave1.z, 0.001f, 0.0f, 1.0f, "%.3f");

	ImGui::Text("Wave 2");
	ImGui::DragFloat2("Direction 2", &wave2.x, 0.01f, -1.0f, 1.0f, "%.2f");
	ImGui::DragFloat("Wavelength 2", &wave2.w, 0.01f, 0.0f, 100.0f, "%.2f");
	ImGui::DragFloat("Steepness 2", &wave2.z, 0.001f, 0.0f, 1.0f, "%.3f");
  }
  ImGui::End();
}

// Update the wave positions - adapted from GPU implementation to test CPU implementation, ie.
// no parallelization.
void Water::Update(std::vector<float> &heights) {
  float waterlevel = dimensions.y;
  float time = (float) glfwGetTime();

  glm::vec4 wave0 = glm::vec4(0.09f, 0.71f, 0.06f, 85.0f);
  glm::vec4 wave1 = glm::vec4(-0.01f, 0.45f, 0.03f, 30.0f);
  glm::vec4 wave2 = glm::vec4(0.18f, 0.32f, 0.06f, 40.0f);

  auto HMIndex = [&](int x, int z) {
	return z * dimensions.x + x;
  };

  auto Map = [&](float value, float min1, float max1, float min2, float max2) {
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
  };

  auto GerstnerWave =
	  [&](glm::vec4 wave, glm::vec3 p, float factor, float ndepth, glm::vec3 &tangent, glm::vec3 &binormal) {
		float steepness = wave.z;
		float wavelength = wave.w * factor;
//    float wavelength = wave.w;

		float k = 2 * PI / wavelength;
		float c = sqrt(9.8 / k);
		glm::vec2 d = glm::normalize(glm::vec2(wave.x, wave.y));
		float f = k * (glm::dot(d, glm::vec2(p.x, p.z)) - c * time);
		float a = steepness / k;

		tangent += glm::vec3(
			-d.x * d.x * (steepness * sin(f)),
			d.x * (steepness * cos(f)),
			-d.x * d.y * (steepness * sin(f))
		);

		binormal += glm::vec3(
			-d.x * d.y * (steepness * sin(f)),
			d.y * (steepness * cos(f)),
			-d.y * d.y * (steepness * sin(f))
		);

		return glm::vec3(
			d.x * (a * cos(f)),
			a * sin(f),
			d.y * (a * cos(f)) + (ndepth * sin(time))
		);
	  };

  for (int i = 0; i < vertices.size(); ++i) {
	Vertex v = vertices[i];
	float heightSample = heights[HMIndex(v.position.x, v.position.z)];

	float depth = waterlevel - heightSample;
	float normalizeddepth = Map(depth, 0.0, waterlevel, 0.0, 1.0);
	float depthfactor = Map(depth, 0.0, waterlevel, 2.0f, 0.5f);

	glm::vec3 p = vertices2[i].position;
	glm::vec3 tangent = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 binormal = glm::vec3(0.0, 0.0, 1.0);

	p += GerstnerWave(wave0, v.position, depthfactor, normalizeddepth, tangent, binormal);
	p += GerstnerWave(wave1, v.position, depthfactor, normalizeddepth, tangent, binormal);
	p += GerstnerWave(wave2, v.position, depthfactor, normalizeddepth, tangent, binormal);

	vertices[i].position = p;
	vertices[i].normal = glm::normalize(glm::cross(binormal, tangent));
  }

  __ChangeData(vertices, indices);
}
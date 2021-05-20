#include "PCHeader.h"
#include "Terrain.h"

Terrain::Terrain(GUILayer *gui,
				 float x,
				 float y,
				 float z,
				 float width,
				 float height,
				 float depth,
				 std::shared_ptr<Utils::Perlin> _perlin,
				 float _resolution)
	: GL::Plane(x, y, z, width, height, depth, _resolution), perlin(_perlin) {
  // Set the shader to use.
  Set(GL::RENDER_TERRAIN);

  // Add GUI elements to GUI layer
  gui->addElement([this]() {
	this->GUIElement("Terrain");
  });
}

// Generates the height thresholds, height map and terrain vertices, indices and normals.
// We also populate the height data for use in other areas of the program.
void Terrain::Generate() {
  vertices.clear();
  indices.clear();

  GenerateHeightThresholds();
  auto maxi = maxHeightThresholds;

  int cellsX = dimensions.x / resolution;
  int cellsZ = dimensions.z / resolution;

  // Height Map
  heightData.dimensions = glm::ivec3(cellsX, 0, cellsZ);
  heightData.yOffset = position.y;
  heightData.resolution = resolution;
  static double yoff = 0.0;
  double zoff = 0.0;
  for (int z = 0; z < heightData.dimensions.z; ++z) {
	double xoff = 0.0;
	std::vector<float> row;
	heightData.heights2D.push_back(row);
	for (int x = 0; x < heightData.dimensions.x; ++x) {
	  float height = minHeightThresholds[z] + ((float) abs(perlin->Noise(5, 0.6f, xoff, yoff, zoff) / 2) * maxi[z]);
	  heightData.heights2D[z].push_back(height);
	  heightData.heights.push_back(height);
	  xoff += 0.01;
	}
	zoff += 0.02;
  }
  yoff += 0.0001;

  // Create Vertices
  glm::vec4 COLOUR = glm::vec4(0.21f, 0.55f, 0.77f, 1.0f);
  for (float z = 0; z < cellsZ; ++z) {
	for (float x = 0; x < cellsX; ++x) {
	  int xp = position.x + x * resolution;
	  int zp = position.z + z * resolution;
	  int yp = position.y + heightData.heights2D[z][x];

	  Vertex v = {{xp, yp, zp}, GetColourFromHeight(yp), {0.0f, 0.0f, 0.0f}};
	  vertices.push_back(v);
	}
  }

  // Indices
  for (int z = 0; z < cellsZ - 1; ++z) {
	int Z = z * (cellsX);
	int ZN = (z + 1) * (cellsX);

	for (int x = 0; x < cellsX - 1; ++x) {

	  indices.push_back(Z + x); // A
	  indices.push_back(ZN + x + 1); // C
	  indices.push_back(ZN + x); // D

	  indices.push_back(ZN + x + 1); // C
	  indices.push_back(Z + x); // A
	  indices.push_back(Z + x + 1); // B
	}
  }

  // Normals
  for (int z = 0; z < cellsZ - 1; ++z) {
	for (int x = 0; x < cellsX - 1; ++x) {
	  vertices[z * cellsZ + x].normal = CalculateNormal(x, z);
	}
  }

  __Init(vertices, indices);
}

// Bind the buffers and send uniform values to GPU.
void Terrain::Begin(GL::Program *program) {
  Bind();

  program->Float("t1", colours.threshold[0]);
  program->Float("t2", colours.threshold[1]);
  program->Float("t3", colours.threshold[2]);
  program->Float("t4", colours.threshold[3]);
  program->Float("t5", colours.threshold[4]);

  program->Vec4("c1", colours.colour1);
  program->Vec4("c2", colours.colour2);
  program->Vec4("c3", colours.colour3);
  program->Vec4("c4", colours.colour4);
  program->Vec4("c5", colours.colour5);
  program->Vec4("def", colours.def);

}

// Calculates the normals of the terrain section.
glm::vec3 Terrain::CalculateNormal(unsigned int x, unsigned int z) {
  auto Index = [&](int x, int z) { return z * (int) (dimensions.z / resolution) + x; };

  if (x == 0) x = 1;
  if (z == 0) z = 1;
  float hl = vertices[Index(x - 1, z)].position.y;
  float hr = vertices[Index(x + 1, z)].position.y;
  float hd = vertices[Index(x, z + 1)].position.y;
  float hu = vertices[Index(x, z - 1)].position.y;
  glm::vec3 n = glm::vec3(hl - hr, 2.0f, hd - hu);
  return glm::normalize(n);
}

// Items to be displayed on the GUI to allow real-time control of the element.
void Terrain::GUIElement(std::string name) {
  ImGui::Begin(name.c_str());
  {
	ImGui::DragFloat("T1", &colours.threshold[0], 0.01f, 0.0f, colours.threshold[1] - 0.01f, "%.2f");
	ImGui::ColorEdit4("C1", &colours.colour1.x);

	ImGui::DragFloat("T2",
					 &colours.threshold[1],
					 0.01f,
					 colours.threshold[0] + 0.01f,
					 colours.threshold[2] - 0.01f,
					 "%.2f");
	ImGui::ColorEdit4("C2", &colours.colour2.x);

	ImGui::DragFloat("T3",
					 &colours.threshold[2],
					 0.01f,
					 colours.threshold[1] + 0.01f,
					 colours.threshold[3] - 0.01f,
					 "%.2f");
	ImGui::ColorEdit4("C3", &colours.colour3.x);

	ImGui::DragFloat("T4",
					 &colours.threshold[3],
					 0.01f,
					 colours.threshold[2] + 0.01f,
					 colours.threshold[4] - 0.01f,
					 "%.2f");
	ImGui::ColorEdit4("C4", &colours.colour4.x);

	ImGui::DragFloat("T5", &colours.threshold[4], 0.01f, colours.threshold[3] + 0.01f, 1.0, "%.2f");
	ImGui::ColorEdit4("C5", &colours.colour5.x);

	ImGui::ColorEdit4("Def", &colours.def.x);

  }
  ImGui::End();
}

// Generates the height thresholds, which are a min and maximum that a height can be at a
// certain point, allowing the terrain to slope up from a sea floor inro mountains.
void Terrain::GenerateHeightThresholds() {
  maxHeightThresholds.clear();

  unsigned int numValues = dimensions.z / resolution;

  for (unsigned int i = 0; i < numValues; ++i) {
	float percent = (i * 1.0f) / (numValues * 1.0f);

	maxHeightThresholds.push_back(percent * dimensions.y);
	minHeightThresholds.push_back(std::max((percent - 0.2f), 0.0f) * dimensions.y);
  }
}

// Calculate the color of a vertex from it's height.
glm::vec4 Terrain::GetColourFromHeight(float height) {
  if (height < 0.3 * dimensions.y) {
	return glm::vec4(0.98, 0.79, 0.14, 1.0);
  } else if (height < 0.32 * dimensions.y) {
	return glm::vec4(1.0, 0.75, 0.46, 1.0);
  } else if (height < 0.6 * dimensions.y) {
	return glm::vec4(0.42, 0.69, 0.30, 1.0);
  } else if (height < 0.9 * dimensions.y) {
	return glm::vec4(0.34, 0.40, 0.45, 1.0);
  }

  return glm::vec4(1.0, 1.0, 1.0, 1.0);
}

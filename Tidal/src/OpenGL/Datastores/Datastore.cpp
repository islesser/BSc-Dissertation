#include "PCHeader.h"
#include "Datastore.h"

void GL::Datastore::Gerstner(std::vector<float> &heights) {
  float waterlevel = 150.0f;
  float time = (float) glfwGetTime();

  glm::vec4 wave0 = glm::vec4(0.09f, 0.71f, 0.06f, 85.0f);
  glm::vec4 wave1 = glm::vec4(-0.01f, 0.45f, 0.03f, 30.0f);
  glm::vec4 wave2 = glm::vec4(0.18f, 0.32f, 0.06f, 40.0f);

  auto HMIndex = [&](int x, int z) {
	return z * 400.0f + x;
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

  for (int i = 0; i < elements.size(); ++i) {
	Vertex v = elements[i];
	float heightSample = heights[HMIndex(v.position.x, v.position.z)];

	float depth = waterlevel - heightSample;
	float normalizeddepth = Map(depth, 0.0, waterlevel, 0.0, 1.0);
	float depthfactor = Map(depth, 0.0, waterlevel, 2.0f, 0.5f);

	glm::vec3 p = elements2[i].position;
	glm::vec3 tangent = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 binormal = glm::vec3(0.0, 0.0, 1.0);

	p += GerstnerWave(wave0, v.position, depthfactor, normalizeddepth, tangent, binormal);
	p += GerstnerWave(wave1, v.position, depthfactor, normalizeddepth, tangent, binormal);
	p += GerstnerWave(wave2, v.position, depthfactor, normalizeddepth, tangent, binormal);

	elements[i].position = p;
  }

  Update();
}
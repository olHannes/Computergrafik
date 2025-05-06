#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glut.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>

#define PRAKTIKUM_2 1;

using namespace glm;

struct Triangle {
	glm::vec3 v0, v1, v2;
};


class SphereTransformations {
public:
	SphereTransformations();
	void generate(int n);
	std::vector<Triangle> renderSphere();
	void increaseN();
	void decreaseN();
	void increaseRadius();
	void decreaseRadius();

private:
	void createInitialSphere();
	void subdivideGrid(int level);
	glm::vec3 midpoint(const glm::vec3& pointA, const glm::vec3& pointB);

	std::vector<Triangle> triangles;
	int n;
	float radius;
};
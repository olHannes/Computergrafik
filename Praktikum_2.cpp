#include "Praktikum_2.h"

SphereTransformations::SphereTransformations()
:n(0),
radius(1.0f),
showAddition(0)
{
	renderSphere();
}


std::vector<Triangle> SphereTransformations::renderSphere() {
	generate(n);
	return triangles;
}



void SphereTransformations::generate(int n) {
	triangles.clear();
	createInitialSphere();

	if (n > 0)
		subdivideGrid(n);
}

void SphereTransformations::increaseN() {
	if (n <= 4) n += 1;
}

void SphereTransformations::decreaseN() {
	if (n > 0) n -= 1;
}

void SphereTransformations::increaseRadius() {
	if (radius < 2) radius += 0.2f;
}

void SphereTransformations::decreaseRadius() {
	if (radius > 0.5f)radius -= 0.2f;
}


void SphereTransformations::createInitialSphere() {
	vec3 top(0, 1, 0);
	vec3 bottom(0, -1, 0);
	vec3 left(-1, 0, 0);
	vec3 right(1, 0, 0);
	vec3 front(0, 0, 1);
	vec3 back(0, 0, -1);

	triangles = {
		{top, front, right},
		{top, right, back},
		{top, back, left},
		{top, left, front},
		{bottom, right, front},
		{bottom, back, right},
		{bottom, left, back},
		{bottom, front, left},
	};

	for (auto& tri : triangles) {
		tri.v0 = normalize(tri.v0) * radius;
		tri.v1 = normalize(tri.v1) * radius;
		tri.v2 = normalize(tri.v2) * radius;
	}
}

glm::vec3 SphereTransformations::midpoint(const glm::vec3& pointA, const glm::vec3& pointB) {
	return normalize((pointA + pointB) * 0.5f);
}

void SphereTransformations::subdivideGrid(int level) {
	std::vector<Triangle> newTriangles;

	//iterate over all triangles
	for (const Triangle& tri : triangles) {
		const glm::vec3& v0 = tri.v0;
		const glm::vec3& v1 = tri.v1;
		const glm::vec3& v2 = tri.v2;

		std::vector<std::vector<glm::vec3>> grid(level + 1);

		for (int i = 0; i <= level; ++i) {
			for (int j = 0; j <= i; ++j) {
				float a = 1.0f - ((float)i / level);
				float b = ((float)(j)) / level;
				float c = 1.0f - a - b;

				glm::vec3 p = normalize(a * v0 + b * v1 + c * v2) * radius;
				grid[i].push_back(p);
			}
		}

		for (int i = 0; i < level; ++i) {
			for (int j = 0; j < i; ++j) {
				newTriangles.push_back({ grid[i][j], grid[i + 1][j], grid[i + 1][j + 1] });
				newTriangles.push_back({ grid[i][j], grid[i + 1][j + 1], grid[i][j + 1] });
			}
			newTriangles.push_back({ grid[i][i], grid[i + 1][i], grid[i + 1][i + 1] });
		}
	}

	triangles = std::move(newTriangles);
}




void SphereTransformations::showAdditions(bool show) {
	this->showAddition = show;
}


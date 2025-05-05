#include "Praktikum_2.h"

SphereTransformations::SphereTransformations(){
	n = 0;
	radius = 3.0f;
	//generate(n);
	createInitialSphere();
}


std::vector<Triangle> SphereTransformations::renderSphere() {
	if (triangles.empty()) {
		generate(n);
	}
	return triangles;
}



void SphereTransformations::generate(int n) {
	triangles.clear();

	for (int i = 0; i < n; ++i) {
		subdivide();
	}
}

void SphereTransformations::increaseN() {
	n = (n <= 4) ? 4 : n += 1;
}

void SphereTransformations::decreaseN() {
	n = (n >= 0) ? 0 : n -= 1;
}

void SphereTransformations::increaseRadius() {

}

void SphereTransformations::decreaseRadius() {

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
}

glm::vec3 SphereTransformations::midpoint(const glm::vec3& pointA, const glm::vec3& pointB) {
	return glm::normalize((pointA + pointB) * 0.5f);
}

void SphereTransformations::subdivide() {
	std::vector<Triangle> newTriangles;
	for (const auto& currentTriangle : triangles) {
		vec3 m0 = midpoint(currentTriangle.v0, currentTriangle.v1);
		vec3 m1 = midpoint(currentTriangle.v1, currentTriangle.v2);
		vec3 m2 = midpoint(currentTriangle.v2, currentTriangle.v0);

		m0 *= radius;
		m1 *= radius;
		m2 *= radius;

		newTriangles.push_back({currentTriangle.v0, m0, m2});
		newTriangles.push_back({m0, currentTriangle.v1});
		newTriangles.push_back({m0, m1, m2});
		newTriangles.push_back({m2, m1, currentTriangle.v2});
	}
	triangles = std::move(newTriangles);
}

#include "Praktikum_2.h"

//########################################################################### Constructor call (init params)
SphereTransformations::SphereTransformations(glm::vec3 pos)
	:n(0),
	radius(1.0f),
	xRotation(0.0f),
	yRotation(0.0f),
	zRotation(0.0f),
	rotationMatrix(mat4(1.0f)),
	absolutePosition(pos)
{
	renderSphere();
}

SphereTransformations::SphereTransformations()
	:n(0),
	radius(1.0f),
	xRotation(0.0f),
	yRotation(0.0f),
	zRotation(0.0f),
	rotationMatrix(mat4(1.0f))
{
	renderSphere();
}


//########################################################################### render Sphere -> create Triangles and return them
std::vector<Triangle> SphereTransformations::renderSphere() {
	generate(n);
	return triangles;
}


//########################################################################### return the coordinate system
std::vector<glm::vec3> SphereTransformations::getCoords() {
	return coords;
}


//########################################################################### generate Sphere
void SphereTransformations::generate(int n) {
	triangles.clear();
	createInitialSphere();
	//createInitialCoords();

	if (n > 0) {
		subdivideGrid(n);
	}
	//transformRotation();
}


//########################################################################### handle User-Input
void SphereTransformations::increaseN() {
	if (n < 4) n += 1;
}

void SphereTransformations::decreaseN() {
	if (n > 0) n -= 1;
}

void SphereTransformations::setN(int pN) {
	if (pN > 0 && pN < 4) this->n = pN;
}
int SphereTransformations::getN() {
	return this->n;
}


void SphereTransformations::increaseRadius() {
	if (radius < 2) radius += 0.2f;
}

void SphereTransformations::decreaseRadius() {
	if (radius > 0.5f)radius -= 0.2f;
}


void SphereTransformations::zoomIn() {
	zIndex += 0.1f;
}
void SphereTransformations::zoomOut() {
	zIndex -= 0.1f;
}



void SphereTransformations::calcRotationMatrix(float rotValue, vec3 axis) {
	rotationMatrix = glm::rotate(rotationMatrix, rotValue, axis);
}

void SphereTransformations::setXRotation() {
	this->xRotation += 0.01;
	calcRotationMatrix(xRotation, { 1,0,0 });
}

void SphereTransformations::setYRotation() {
	this->yRotation += 0.01;
	calcRotationMatrix(yRotation, { 0,1,0 });
}

void SphereTransformations::setZRotation() {
	this->zRotation += 0.01;
	calcRotationMatrix(zRotation, { 0,0,1 });
}

void SphereTransformations::resetRotation() {
	this->xRotation = 0;
	this->yRotation = 0;
	this->zRotation = 0;
	rotationMatrix = mat4(1.0f);
}


//########################################################################### Create the initial Sphere
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


//########################################################################### Create the initial coordinate system
void SphereTransformations::createInitialCoords() {
	coords = {
		{ 0,0,0 },
		{ 1,0,0 },
		{ 0,0,0 },
		{ 0,1,0 },
		{ 0,0,0 },
		{ 0,0,1 }
	};
}

void SphereTransformations::createInitialCoords(const glm::vec3& startAxis, const glm::vec3& endAxis) {
	coords = { startAxis, endAxis };
}

//########################################################################### Get the midpoint between two points
glm::vec3 SphereTransformations::midpoint(const glm::vec3& pointA, const glm::vec3& pointB) {
	return normalize((pointA + pointB) * 0.5f);
}


//########################################################################### calc Tessellation
void SphereTransformations::subdivideGrid(int level) {
	std::vector<Triangle> newTriangles;
	level = level + 1;

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


//########################################################################### Transform the Sphere and coordinate system
void SphereTransformations::transformRotation() {
	std::vector<Triangle> newTriangles;
	
	for (const auto& triangle : triangles) {
		glm::vec3 v0_rotated = glm::vec3(rotationMatrix * glm::vec4(triangle.v0, 1.0f));
		glm::vec3 v1_rotated = glm::vec3(rotationMatrix * glm::vec4(triangle.v1, 1.0f));
		glm::vec3 v2_rotated = glm::vec3(rotationMatrix * glm::vec4(triangle.v2, 1.0f));

		Triangle tNew;
		tNew.v0 = v0_rotated;
		tNew.v1 = v1_rotated;
		tNew.v2 = v2_rotated;
		newTriangles.push_back(tNew);
	}

	triangles = std::move(newTriangles);


	std::vector<glm::vec3> tempCoords;
	for (const auto& point : coords) {
		glm::vec3 point_rotated = glm::vec3(rotationMatrix * glm::vec4(point, 1.0f));
		tempCoords.push_back(point_rotated);
	}
	coords = std::move(tempCoords);
}


glm::vec3 SphereTransformations::rotateTranslationVector(glm::vec3 vec) {
	glm::vec3 rotated = glm::vec3(rotationMatrix * glm::vec4(vec, 1.0f));

	return rotated;
}


//########################################################################### calc the normal lines and return them
std::vector<glm::vec3> SphereTransformations::generateNormalLines() {
	std::vector<glm::vec3> lines;
	float scale = 0.2f;

	for (const auto& tri : triangles) {
		lines.push_back(tri.v0);
		lines.push_back(tri.v0 + scale * tri.v0);

		lines.push_back(tri.v1);
		lines.push_back(tri.v1 + scale * tri.v1);

		lines.push_back(tri.v2);
		lines.push_back(tri.v2 + scale * tri.v2);
	}
	
	return lines;
}

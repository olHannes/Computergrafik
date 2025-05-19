#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glut.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	
	void zoomIn();
	void zoomOut();

	void setXRotation();
	void setYRotation();
	void setZRotation();
	void resetRotation();


	std::vector<glm::vec3> generateNormalLines();
	std::vector<glm::vec3> getCoords();

	float zIndex;


private:
	void createInitialSphere();
	void createInitialCoords();

	void subdivideGrid(int level);
	glm::vec3 midpoint(const glm::vec3& pointA, const glm::vec3& pointB);

	void transformRotation();
	void calcRotationMatrix(float rotValue, vec3 axis);
	glm::mat4 rotationMatrix;

	std::vector<Triangle> triangles;
	std::vector<glm::vec3> coords;

	int n;
	float radius;
	
	float xRotation;
	float yRotation;
	float zRotation;
};
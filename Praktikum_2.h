#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glut.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PRAKTIKUM_2 0

using namespace glm;

struct Triangle {
	glm::vec3 v0, v1, v2;
};


class SphereTransformations {
public:
	SphereTransformations(glm::vec3 pos);
	SphereTransformations();

	void generate(int n);
	std::vector<Triangle> renderSphere();
	
	void increaseN();
	void decreaseN();
	void setN(int pN);
	int getN();
	
	void increaseRadius();
	void decreaseRadius();

	void setRadius(float rad) {
		this->radius = rad;
	}
	
	void zoomIn();
	void zoomOut();

	void setXRotation(float pAngle = 0.1f);
	void setYRotation(float pAngle = 0.1f);
	void setZRotation(float pAngle=0.1f);
	void resetRotation();

	void setXRotationValue(float pValue) {
		this->xRotation = pValue;
	}
	void setYRotationValue(float pValue) {
		this->yRotation = pValue;
	}
	void setZRotationValue(float pValue) {
		this->zRotation = pValue;
	}

	std::vector<glm::vec3> generateNormalLines();
	std::vector<glm::vec3> getCoords();

	float zIndex;
		
	std::vector<Triangle> getTriangles() {
		return this->triangles;
	}

	void setTriangles(std::vector<Triangle> tri) {
		this->triangles = tri;
	}


	void transformRotation(mat4 pRotationMatrix);
	glm::vec3 rotateTranslationVector(glm::vec3 vec, glm::mat4 pRotationMatrix);
	glm::vec3 absolutePosition;
	glm::vec3 oldPosition;


	glm::mat4 getRotationMatrix() {
		return this->rotationMatrix;
	}



	void rotateAbsolute(mat4 pRotationMatrix) {
		this->absolutePosition = glm::vec3(pRotationMatrix * glm::vec4(this->absolutePosition, 1.0f));
	}


private:
	void createInitialSphere();
	void createInitialCoords(bool prak3=true);

	void subdivideGrid(int level);
	glm::vec3 midpoint(const glm::vec3& pointA, const glm::vec3& pointB);

	void calcRotationMatrix(float rotValue, vec3 axis);

	std::vector<Triangle> triangles;
	std::vector<glm::vec3> coords;

	int n;
	float radius;
public:
	glm::mat4 rotationMatrix;
	float xRotation;
	float yRotation;
	float zRotation;
};
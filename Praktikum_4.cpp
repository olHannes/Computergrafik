#include "Praktikum_4.h"


PolygonMesh::PolygonMesh()
: color(glm::vec3(1.0f, 1.0f, 1.0f) )
{
}

void PolygonMesh::setColor(glm::vec3& pColor) {
	this->color = pColor;
}
glm::vec3 PolygonMesh::getColor() {
	return this->color;
}

std::vector<Vertex>& PolygonMesh::getVertices() {
	return vertices;
}
std::vector<Normal>& PolygonMesh::getNormals() {
	return normals;
}


/*
std::vector<int>& PolygonMesh::getTriangleIndices() {
	return {0};
}
*/




bool PolygonMesh::loadOBJ(const std::string& filename) {
	return false;
}


void PolygonMesh::triangulate() {

}

void PolygonMesh::triangulateFace(const Face& face, std::vector<int>& outIndices) {

}


void PolygonMesh::fitToScene(float targetSize) {

}

void PolygonMesh::computeBoundingBox(glm::vec3& minOut, glm::vec3& maxOut) {

}


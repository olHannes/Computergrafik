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
std::vector<Face>& PolygonMesh::getFaces() {
	return faces;
}


/*
std::vector<int>& PolygonMesh::getTriangleIndices() {
	return {0};
}
*/




bool PolygonMesh::loadOBJ(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                vertices.emplace_back(x, y, z);
            }
            else if (type == "f") {
                Face face;
                int index;
                while (iss >> index) {
                    // OBJ indiziert ab 1, C++ ab 0
                    face.vertexIndices.push_back(index - 1);
                }
                faces.push_back(face);
            }
        }
        file.close();
        return true;
    }



void PolygonMesh::triangulate() {

}

void PolygonMesh::triangulateFace(const Face& face, std::vector<int>& outIndices) {

}


void PolygonMesh::fitToScene(float targetSize) {

}

void PolygonMesh::computeBoundingBox(glm::vec3& minOut, glm::vec3& maxOut) {

}


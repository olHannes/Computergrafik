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


std::vector<int>& PolygonMesh::getTriangleIndices() {
    return triangleIndices;
}

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
            } else if (type == "vn") {
                Normal normal;
                float x, y, z;
                iss >> x >> y >> z;
                normals.emplace_back(x, y, z);
            } else if (type == "f") {
                Face face;
                std::string token;
                while (iss >> token) {
                    std::replace(token.begin(), token.end(), '/', ' '); // Ersetze / durch Leerzeichen
                    std::istringstream tokenStream(token);
                    int vIdx, tIdx, nIdx;
                    tokenStream >> vIdx >> tIdx >> nIdx;
                    face.vertexIndices.push_back(vIdx);
                    face.normalIndices.push_back(nIdx);
                }
                faces.push_back(face);
            }
        }
        file.close();
        return true;
    }



void PolygonMesh::triangulate() {
    std::vector<Face> newFaces;

    for (const Face& face : faces) {
        triangulateFace(face, newFaces);
    }

    faces = std::move(newFaces);
}

void PolygonMesh::triangulateFace(const Face& face, std::vector<Face>& outFaces) {
    size_t vertexCount = face.vertexIndices.size();
    if (vertexCount < 3) return;

    if (vertexCount == 3) {
        outFaces.push_back(face);
        return;
    }

    glm::vec3 center(0.0f);
    for (int idx : face.vertexIndices) {
        center += vertices[idx].position;
    }
    center /= static_cast<float>(vertexCount);

    vertices.emplace_back(center.x, center.y, center.z);
    int centerIndex = static_cast<int>(vertices.size()) - 1;

    if (!face.normalIndices.empty()) {
        glm::vec3 avgNormal(0.0f);
        for (int nIdx : face.normalIndices) {
            avgNormal += normals[nIdx].direction;
        }
        avgNormal = glm::normalize(avgNormal);
        normals.emplace_back(avgNormal.x, avgNormal.y, avgNormal.z);
    }
    int centerNormalIndex = normals.empty() ? -1 : static_cast<int>(normals.size()) - 1;

    for (size_t i = 0; i < vertexCount; ++i) {
        int v0 = face.vertexIndices[i];
        int v1 = face.vertexIndices[(i + 1) % vertexCount];

        Face triangle;
        triangle.vertexIndices = { centerIndex, v0, v1 };

        if (!face.normalIndices.empty() && centerNormalIndex != -1) {
            int n0 = face.normalIndices[i];
            int n1 = face.normalIndices[(i + 1) % vertexCount];
            triangle.normalIndices = { centerNormalIndex, n0, n1 };
        }

        outFaces.push_back(triangle);
    }
}




void PolygonMesh::fitToScene(float targetSize) {

}

BoundingBox PolygonMesh::computeBoundingBox() {
    if (vertices.empty())
        throw std::runtime_error("Vertices are empty");

    BoundingBox pBox;
    pBox.xMin = 0;
    pBox.xMax = 0;
    pBox.yMin = 0;
    pBox.yMax = 0;

    for (size_t i = 1; i < vertices.size(); ++i) {
        if (vertices[i].position.x < vertices[pBox.xMin].position.x) {
            pBox.xMin = i;
        }
        if (vertices[i].position.x > vertices[pBox.xMax].position.x) {
            pBox.xMax = i;
        }
        if (vertices[i].position.y < vertices[pBox.yMin].position.y) {
            pBox.yMin = i;
        }
        if (vertices[i].position.y > vertices[pBox.yMax].position.y) {
            pBox.yMax = i;
        }
    }
    return pBox;
}


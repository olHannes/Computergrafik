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
    bool hasNormals = false;
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
                hasNormals = true;
                Normal normal;
                float x, y, z;
                iss >> x >> y >> z;
                normals.emplace_back(x, y, z);
            } else if (type == "f") {
                Face face;
                std::string token;
                while (iss >> token) {
                    std::replace(token.begin(), token.end(), '/', ' ');
                    std::istringstream tokenStream(token);
                    int vIdx, tIdx, nIdx;
                    tokenStream >> vIdx >> tIdx >> nIdx;
                    face.vertexIndices.push_back(vIdx -1);
                    face.normalIndices.push_back(nIdx -1);
                }
                faces.push_back(face);
                if (face.normalIndices.size() != 0) hasNormals = true;
            }
        }
        file.close();
        if (!hasNormals) {
            std::cout << "Model hat keine Normalen";
        }
       
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
    std::vector<Triangle> scaledTriangles;

    for (const auto& tri : objHandle.sphere.getTriangles()) {
        Triangle newTri;
        newTri.v0 = tri.v0 * targetSize;
        newTri.v1 = tri.v1 * targetSize;
        newTri.v2 = tri.v2 * targetSize;
        scaledTriangles.push_back(newTri);
    }

    objHandle.sphere.setTriangles(scaledTriangles);
}


std::vector<glm::vec3> PolygonMesh::computeBoundingBox() {
    const std::vector<Triangle>& triangles = objHandle.sphere.getTriangles();
    if (triangles.empty())
        throw std::runtime_error("No triangles available");

    glm::vec3 min = triangles[0].v0;
    glm::vec3 max = triangles[0].v0;

    auto updateMinMax = [&](const glm::vec3& v) {
        min = glm::min(min, v);
        max = glm::max(max, v);
        };

    for (const auto& tri : triangles) {
        updateMinMax(tri.v0);
        updateMinMax(tri.v1);
        updateMinMax(tri.v2);
    }

    std::vector<glm::vec3> boundingBoxCorners;

    boundingBoxCorners.push_back(glm::vec3(min.x, min.y, min.z));
    boundingBoxCorners.push_back(glm::vec3(max.x, min.y, min.z));
    boundingBoxCorners.push_back(glm::vec3(min.x, max.y, min.z));
    boundingBoxCorners.push_back(glm::vec3(max.x, max.y, min.z));
    boundingBoxCorners.push_back(glm::vec3(min.x, min.y, max.z));
    boundingBoxCorners.push_back(glm::vec3(max.x, min.y, max.z));
    boundingBoxCorners.push_back(glm::vec3(min.x, max.y, max.z));
    boundingBoxCorners.push_back(glm::vec3(max.x, max.y, max.z));

    return boundingBoxCorners;
}








void PolygonMesh::convertFaceToTriangleAndNormal() {
    std::vector<Triangle> pTriangles;
    std::vector<glm::vec3> pNormalLines;

    for (auto& pFace : faces) {
        Triangle newTri;

        newTri.v0 = vertices[pFace.vertexIndices[2]].position;
        newTri.v1 = vertices[pFace.vertexIndices[1]].position;
        newTri.v2 = vertices[pFace.vertexIndices[0]].position;
        pTriangles.push_back(newTri);


        pNormalLines.push_back (normals[pFace.normalIndices[0]].direction);
        pNormalLines.push_back (normals[pFace.normalIndices[1]].direction);
        pNormalLines.push_back (normals[pFace.normalIndices[2]].direction);
    }

    this->objHandle.sphere.setTriangles(pTriangles);
    this->sphereObjNormalLines = pNormalLines;

    fitToScene();
}


void PolygonMesh::convertNormals() {
    std::vector<glm::vec3> tempNormals;

    int k = 0;
    for (int i = 0; i < this->objHandle.sphere.getTriangles().size(); i++) {
        
        tempNormals.push_back(this->objHandle.sphere.getTriangles()[i].v0);
        tempNormals.push_back(this->sphereObjNormalLines[k]);
        k++;

        tempNormals.push_back(this->objHandle.sphere.getTriangles()[i].v1);
        tempNormals.push_back(this->sphereObjNormalLines[k]);
        k++;

        tempNormals.push_back(this->objHandle.sphere.getTriangles()[i].v2);
        tempNormals.push_back(this->sphereObjNormalLines[k]);
        k++;
    }
}



#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Praktikum_3.h"

#define PRAKTIKUM_4 1
struct Vertex {
    glm::vec3 position;

    Vertex() = default;

    Vertex(float x, float y, float z) : position(x, y, z) {}
};

struct Normal {
    glm::vec3 direction;

    Normal() = default;

    Normal(float x, float y, float z) : direction(x, y, z) {}
};

struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> normalIndices;
};


struct BoundingBox {
    int xMin;
    int xMax;
    int yMin;
    int yMax;
};


class PolygonMesh {
public:
    PolygonMesh();

    bool loadOBJ(const std::string& filename);

    void triangulate();

    void fitToScene(float targetSize = 1.0f);

    std::vector<Vertex>& getVertices();
    std::vector<Normal>& getNormals();
    std::vector<Face>& getFaces();

    std::vector<int>& getTriangleIndices();

    void setColor(glm::vec3& pColor);
    glm::vec3 getColor();

    BoundingBox computeBoundingBox();

    //convert to Sphere
    void convertFaceToTriangleAndNormal();
    void convertNormals();
    //

    ObjectBodyHandler objHandle;
    std::vector<glm::vec3> sphereObjNormalLines;
private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<Face> faces;

    std::vector<int> triangleIndices;

    glm::vec3 color;

    void triangulateFace(const Face& face, std::vector<Face>& outFaces);

};

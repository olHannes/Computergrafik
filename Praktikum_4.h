#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#define PRAKTIKUM_4 1
struct Vertex {
    glm::vec3 position;
};

struct Normal {
    glm::vec3 direction;
};

struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> normalIndices;
};

class PolygonMesh {
public:
    PolygonMesh();

    bool loadOBJ(const std::string& filename);

    void triangulate();

    void fitToScene(float targetSize = 1.0f);

    std::vector<Vertex>& getVertices();
    std::vector<Normal>& getNormals();

    std::vector<int>& getTriangleIndices();

    void setColor(const glm::vec3& color);
    glm::vec3 getColor() const;

    void computeBoundingBox(glm::vec3& minOut, glm::vec3& maxOut) const;

private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<Face> faces;

    std::vector<int> triangleIndices;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    void triangulateFace(const Face& face, std::vector<int>& outIndices) const;
};

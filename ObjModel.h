#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

class Vertex {
public:
    float x, y, z;
    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Face {
public:
    std::vector<int> indices; // Indizes zu den Eckpunkten
};

class OBJModel {
public:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    bool load(const std::string& filename) {
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
                    face.indices.push_back(index - 1);
                }
                faces.push_back(face);
            }
        }
        file.close();
        return true;
    }
};
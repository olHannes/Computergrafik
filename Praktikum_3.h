#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Praktikum_2.h"

#define PRAKTIKUM_3 1

float const globalX = 0;
float const globalY = 0;
float const globalZ = 0;



class ObjectBodyHandler {
public:
    ObjectBodyHandler();

    bool lineVisible;
    bool bodyRotation;

    void setLineVisible(bool pVisible) {
        this->lineVisible = pVisible;
    }
    void setBodyRotation(bool pRotation) {
        this->bodyRotation = pRotation;
    }

    /*
    float parentDistance;
    void setParentDistance(float pDistance) {
        this->parentDistance = pDistance;
    }
    */
    
    glm::mat4 globalRotationMatrix; //rotation um die Sonne
    

    SphereTransformations sphere; //Sphere Objekt
    std::vector<glm::vec3> line; //Linie


    void setSphereColor(const glm::vec3& color) {
        this->sphereColor = color;
    }
    vec3 getSphereColor() {
        return this->sphereColor;
    }

    glm::vec3 sphereColor = glm::vec3(1.0f);
    const glm::vec3 lineColor = glm::vec3(0.0f);


    std::vector<ObjectBodyHandler> childrenObjects; //Liste an Kindern
    ObjectBodyHandler* parentObject = nullptr;  // Eltern Objekt


    ObjectBodyHandler* getParentObject() {
        return parentObject;
    }
    void setParentObject(ObjectBodyHandler* parent) {
        this->parentObject = parent;
    }




    void transformTranslation(glm::vec3 vec);

    void render();
    void renderObject();

};


/*
TODO:

1. getter-Methode für die Linie
2. Farben in ObjectBodyHandler
3. Steuerung der Rotationsgeschwindigkeit
4. Schräglage des 2. Planeten
5. dynamischer Radius

*/
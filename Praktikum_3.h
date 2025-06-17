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

    void setLineVisible(bool pVisible);
    bool getLineVisible();

    void setBodyRotation(bool pRotation);
    bool getBodyRotation();
    
    mat4 globalRotationMatrix; //rotation um die Sonne
    

    SphereTransformations sphere; //Sphere Objekt
    std::vector<glm::vec3> line; //Linie


    void setSphereColor(const glm::vec3& color);
    vec3 getSphereColor();

    void setLineColor(const vec3& color);
    vec3 getLineColor();

    vec3 sphereColor = vec3(1.0f);
    vec3 lineColor = vec3(0.0f);


    std::vector<ObjectBodyHandler*> childrenObjects; //Liste an Kindern
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
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

    float parentDistance;
    void setParentDistance(float pDistance) {
        this->parentDistance = pDistance;
    }

    
    glm::mat4 globalRotationMatrix; //rotation um die Sonne
    

    SphereTransformations sphere; //Sphere Objekt
    std::vector<glm::vec3> line; //Linie


    std::vector<ObjectBodyHandler> childrenObjects; //Liste an Kindern
    ObjectBodyHandler* parentObject = nullptr;  // Eltern Objekt


    ObjectBodyHandler* getParentObject() {
        return parentObject;
    }
    void setParentObject(ObjectBodyHandler* parent) {
        this->parentObject = parent;
    }




    void transformTranslation(glm::vec3 vec) {
        std::vector<Triangle> tris = sphere.getTriangles();
        for (auto& tri : tris) {
            tri.v0 = tri.v0 - vec;
            tri.v1 = tri.v1 - vec;
            tri.v2 = tri.v2 - vec;
        }
        sphere.setTriangles(tris);
    }


    void render() {
        this->renderObject();
        for (auto& obj : childrenObjects) {
            obj.render();
        }
    }

    void renderObject() {
        //step 1: sphere tesselieren mit n=4
        if (bodyRotation) {
            sphere.setYRotationValue(0.1f);
        }

        //Erstellung und lokale Drehung
        sphere.renderSphere(); 
        sphere.transformRotation();

        // step 2: sphere (und alle children) global um den parent rotieren
        if (this->getParentObject() != nullptr) {
            //globale Rotation (alles außer die Sonne)
            
            /*
            1. Translation in den Ursprung (der Sonne) -> verschoben um den vektor (ursprungspunkt - absolute Koordinate)
            2. Rotation um die y-Achse
            3. Translation zurück
            */
            glm::vec3 translationVector = sphere.absolutePosition;
            transformTranslation(translationVector);
            sphere.transformRotation();
            translationVector = sphere.rotateTranslationVector(translationVector);
            sphere.absolutePosition = sphere.rotateTranslationVector(sphere.absolutePosition);
            transformTranslation(-translationVector);


            if (this->getParentObject()->getParentObject() != nullptr) {
                //Rotation um den this->getParentObject()
                //nur Monde
                /*
                1. Mond um den Planeten rotieren
                    1.1. Translation in den Ursprung (des parent planeten)
                    1.2. Rotation um die y-Achse
                    1.3. Translation zurück
                */
                glm::vec3 absolutePosParent = this->getParentObject()->sphere.absolutePosition;
                translationVector = sphere.absolutePosition - absolutePosParent;
                transformTranslation(translationVector);

                sphere.transformRotation();
                translationVector = sphere.rotateTranslationVector(translationVector);
                sphere.absolutePosition = sphere.rotateTranslationVector(sphere.absolutePosition);
                transformTranslation(-translationVector);
            }
        }

    }

};



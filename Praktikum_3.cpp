#include "Praktikum_3.h"
#include <iostream>

using namespace std;
ObjectBodyHandler::ObjectBodyHandler()
    : globalRotationMatrix(mat4(1.0f))
    , bodyRotation(false)
    , lineVisible(false)
    , yRotationValue(0.01f)
    , inclined(false)
{
    this->sphere = SphereTransformations();
    sphere.renderSphere();
}



/*
Getter / Setter for Line Visibility and local rotation
*/
void ObjectBodyHandler::setLineVisible(bool pVisible) {
    this->lineVisible = pVisible;
}
bool ObjectBodyHandler::getLineVisible() {
    return this->lineVisible;
}

void ObjectBodyHandler::setBodyRotation(bool pRotation) {
    this->bodyRotation = pRotation;
    if(pRotation)
        this->sphere.setYRotationValue(0.2f);
}
bool ObjectBodyHandler::getBodyRotation() {
    return this->bodyRotation;
}

void ObjectBodyHandler::setInclinedStatus(bool pStatus) {
    this->inclined = pStatus;
}
bool ObjectBodyHandler::getInclinedStatus() {
    return this->inclined;
}


/*
Getter / Setter for Color
*/
void ObjectBodyHandler::setSphereColor(const glm::vec3& color) {
    this->sphereColor = color;
}
vec3 ObjectBodyHandler::getSphereColor() {
    return this->sphereColor;
}

void ObjectBodyHandler::setLineColor(const vec3& color) {
    this->lineColor = color;
}
vec3 ObjectBodyHandler::getLineColor() {
    return this->lineColor;
}



void ObjectBodyHandler::calcRotationMatrix() {
    glm::mat4 identity(1.0f);
    globalRotationMatrix = identity;
    globalRotationMatrix = glm::rotate(globalRotationMatrix, yRotationValue, glm::vec3(0, 1, 0));
}


/*
Helper Function: Translation
*/
vec3 ObjectBodyHandler::transformTranslation(glm::vec3 translationVector, glm::vec3 pPoint) {
    
    vec3 temp = pPoint - translationVector;
    return temp;
}


/*
Recursive render-call
*/
void ObjectBodyHandler::render() {
    this->renderObject();
    for (auto* obj : childrenObjects) {
        obj->render();
    }
}


/*
Render the current sphere 
*/
void ObjectBodyHandler::renderObject() {
    sphere.renderSphere();
    calcRotationMatrix();


    if (this->getParentObject() != nullptr) {

        if (this->getParentObject()->getParentObject() == nullptr) {
            //wir sind verfickt nochmal in einem planeten
            glm::vec3 pos = sphere.absolutePosition;
            sphere.oldPosition = pos;
            pos = sphere.rotateTranslationVector(pos, globalRotationMatrix);
            sphere.absolutePosition = pos;
            //der drecksplanet wird um die sonne rotiert -> funktioniert schon
        }
        else {
            // hier ist ein beschissener mond
            vec3 moonPos = this->sphere.absolutePosition;
            vec3 planetPos = this->getParentObject()->sphere.absolutePosition;
            
            
            //-> verschiebe den mond wie den planeten von vorher
            ObjectBodyHandler* planet = this->getParentObject();
            vec3 oldPosPlanet = planet->sphere.oldPosition;
            vec3 absPosPlanet = planet->sphere.absolutePosition;


            moonPos = moonPos + (absPosPlanet - oldPosPlanet);

            //-> verschiebe den planeten in 0,0,0 (gleiche translation für den mond)
            moonPos -= absPosPlanet;
            
            //-> rotiere den mond
            moonPos = sphere.rotateTranslationVector(moonPos, globalRotationMatrix);

            //-> verschiebe den mond zurück
            moonPos += absPosPlanet;

            this->sphere.absolutePosition = moonPos;

        }
    }

    if (this->inclined) {
        glm::mat4 inclination = glm::rotate(glm::mat4(1.0f), 0.78f, glm::vec3(1, 0, 0));
        sphere.transformRotation(inclination);
    }
}



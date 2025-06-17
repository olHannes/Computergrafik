#include "Praktikum_3.h"
#include <iostream>

using namespace std;
ObjectBodyHandler::ObjectBodyHandler()
    : globalRotationMatrix(mat4(1.0f))
    , bodyRotation(false)
    , lineVisible(false)
    , yRotationValue(0.01f)
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
Helper Function: Translation of all Triangles
*/
void ObjectBodyHandler::transformTranslation(glm::vec3 vec) {
    std::vector<Triangle> tris = sphere.getTriangles();
    for (auto& tri : tris) {
        tri.v0 = tri.v0 - vec;
        tri.v1 = tri.v1 - vec;
        tri.v2 = tri.v2 - vec;
    }
    sphere.setTriangles(tris);
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
    //step 1: sphere creation
    sphere.renderSphere();
    sphere.transformRotation(sphere.getRotationMatrix());

    // step 2: sphere (und alle children) global um den parent rotieren
    if (this->getParentObject() != nullptr) {
        //globale Rotation (alles außer die Sonne)

        /*
        1. Translation in den Ursprung (der Sonne) -> verschoben um den vektor (ursprungspunkt - absolute Koordinate)
        2. Rotation um die y-Achse
        3. Translation zurück
        */
        calcRotationMatrix();

        glm::vec3 translationVector = sphere.absolutePosition;
        transformTranslation(translationVector);

        sphere.transformRotation(this->globalRotationMatrix);

        translationVector = sphere.rotateTranslationVector(translationVector, this->globalRotationMatrix);
        sphere.absolutePosition = sphere.rotateTranslationVector(sphere.absolutePosition, this->globalRotationMatrix);
        transformTranslation(-translationVector);

        if (this->getParentObject()->getParentObject() != nullptr) {
            //Rotation um den this->getParentObject()
            //nur Monde
            /*
            1. Mond um den Planeten rotieren
                1.1. Translation in den "Ursprung" (=> Verschiebung des parents in den 0-Punkt & gleiche Verschiebung des Mondes)
                1.2. Rotation um die y-Achse
                1.3. Translation zurück
            */


            vec3 parentPosition = this->getParentObject()->sphere.absolutePosition;
            vec3 moonPosition = this->sphere.absolutePosition;
            vec3 toOrigin = -parentPosition;
            transformTranslation(toOrigin);
            sphere.absolutePosition += toOrigin;

            sphere.transformRotation(this->globalRotationMatrix);
            sphere.absolutePosition = sphere.rotateTranslationVector(sphere.absolutePosition, this->globalRotationMatrix);

            transformTranslation(-toOrigin);
            sphere.absolutePosition -= toOrigin;


        }
    }

}
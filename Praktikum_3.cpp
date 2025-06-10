#include "Praktikum_3.h"
#include <iostream>

using namespace std;
ObjectBodyHandler::ObjectBodyHandler()
{
    //this->sphere = SphereTransformations(glm::vec3(0.0f, 0.0f,0.0f));
    this->sphere = SphereTransformations();
    sphere.renderSphere();
}
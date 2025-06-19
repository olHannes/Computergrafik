#include <iostream>
#include <vector>

#include <GL/glew.h>
//#include <GL/gl.h> // OpenGL header not necessary, included by GLEW
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "GLSLProgram.h"    
#include "GLTools.h"

#include "Praktikum_1.h"
#include "Praktikum_2.h"
#include "Praktikum_3.h"

#define DEFAULT 0 //Flag to show Triangle and Quad (default project)

// Standard window width
const int WINDOW_WIDTH = 640;
// Standard window height
const int WINDOW_HEIGHT = 480;
// GLUT window id/handle
int glutID = 0;

cg::GLSLProgram program;

glm::mat4x4 view;
glm::mat4x4 projection;

float zNear = 0.1f;
float zFar = 100.0f;

/*
Struct to hold data for object rendering.
*/
class Object
{
public:
    inline Object()
        : vao(0), positionBuffer(0), colorBuffer(0), indexBuffer(0)
#if PRAKTIKUM_2 == 1 || PRAKTIKUM_3 == 1
        ,linesVAO(0), linesPositionBuffer(0), linesColorBuffer(0)
#endif //Praktikum_2 / Praktikum_3
    {
    }
    inline ~Object() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &indexBuffer);
        glDeleteBuffers(1, &colorBuffer);
        glDeleteBuffers(1, &positionBuffer);

#if PRAKTIKUM_2 == 1 || PRAKTIKUM_3 == 1
        glDeleteVertexArrays(1, &linesVAO);
        glDeleteBuffers(1, &linesPositionBuffer);
        glDeleteBuffers(1, &linesColorBuffer);
#endif //Praktikum_2 / Praktikum_3
    }

    // Hauptobjekt
    GLuint vao;
    GLuint positionBuffer;
    GLuint colorBuffer;
    GLuint indexBuffer;
    glm::mat4x4 model;

#if PRAKTIKUM_2 == 1 || PRAKTIKUM_3 == 1
    // Koordinatensystem
    GLuint linesVAO;
    GLuint linesPositionBuffer;
    GLuint linesColorBuffer;
#endif //Praktikum_2 / Praktikum_3
};





#if DEFAULT == 1
    Object triangle;
    Object quad;
#endif

#if PRAKTIKUM_2 == 1
    SphereTransformations sphere = SphereTransformations(glm::vec3(-0.5f, 0, 0));
    Object sphereObject;

    Object sphereNormalsObject;
    bool showCoords = true;
    bool showNormals = true;
#endif

#if PRAKTIKUM_3 == 1
    ObjectBodyHandler sun;
    ObjectBodyHandler planet1;
    ObjectBodyHandler moon1;
    ObjectBodyHandler planet2;
    ObjectBodyHandler moon2;

    std::vector<Object> bodyList;
    Object sunBody;
    Object planet1Body;
    Object planet2Body;
    Object moon1Body;
    Object moon2Body;


    void glmInit(Object& body, ObjectBodyHandler obj, bool drawYAxisOnly = false) {
        SphereTransformations sphere = obj.sphere;
        std::vector<Triangle>& tris = sphere.getTriangles();

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> colors;
        std::vector<GLushort> indices;

        GLuint vertexIndex = 0;
        for (const auto& tri : tris) {
            vertices.push_back(tri.v0);
            vertices.push_back(tri.v1);
            vertices.push_back(tri.v2);

            colors.push_back(obj.getSphereColor());
            colors.push_back(obj.getSphereColor());
            colors.push_back(obj.getSphereColor());

            indices.push_back(vertexIndex++);
            indices.push_back(vertexIndex++);
            indices.push_back(vertexIndex++);
        }

        GLuint programId = program.getHandle();
        GLuint pos;


        // === VAO für Hauptobjekt ===
        glGenVertexArrays(1, &body.vao);
        glBindVertexArray(body.vao);

        glGenBuffers(1, &body.positionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, body.positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        pos = glGetAttribLocation(programId, "position");
        glEnableVertexAttribArray(pos);
        glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &body.colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, body.colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
        pos = glGetAttribLocation(programId, "color");
        glEnableVertexAttribArray(pos);
        glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &body.indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, body.indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);

        // === Optional: Nur Y-Achse anzeigen ===
        if (drawYAxisOnly) {
            std::vector<glm::vec3> coordLines = sphere.getCoords();
            if (coordLines.size() >= 2) {
                std::vector<glm::vec3> yAxisLine = {
                    coordLines[2], coordLines[3]
                };

                std::vector<glm::vec3> yAxisColor = {
                    obj.lineColor,
                    obj.lineColor
                };


                glGenVertexArrays(1, &body.linesVAO);
                glBindVertexArray(body.linesVAO);

                glGenBuffers(1, &body.linesPositionBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, body.linesPositionBuffer);
                glBufferData(GL_ARRAY_BUFFER, yAxisLine.size() * sizeof(glm::vec3), yAxisLine.data(), GL_STATIC_DRAW);
                pos = glGetAttribLocation(programId, "position");
                glEnableVertexAttribArray(pos);
                glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glGenBuffers(1, &body.linesColorBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, body.linesColorBuffer);
                glBufferData(GL_ARRAY_BUFFER, yAxisColor.size() * sizeof(glm::vec3), yAxisColor.data(), GL_STATIC_DRAW);
                pos = glGetAttribLocation(programId, "color");
                glEnableVertexAttribArray(pos);
                glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glBindVertexArray(0);
            }
        }

        // === Modellmatrix setzen ===
        body.model = glm::translate(glm::mat4(1.0f), sphere.absolutePosition);
    }


    void glmRender(const Object& body, SphereTransformations& sphere, bool showYAxisOnly = false) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glm::mat4 mvp = projection * view * body.model;

        program.use();
        program.setUniform("mvp", mvp);

        // === Hauptobjekt zeichnen ===
        glBindVertexArray(body.vao);
        glDrawElements(GL_TRIANGLES, sphere.renderSphere().size() * 3, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);

        // === Optional Y-Achse zeichnen ===
        if (showYAxisOnly) {
            glBindVertexArray(body.linesVAO);
            glDrawArrays(GL_LINES, 0, 2); // Nur ein Linienpaar (Y-Achse)
            glBindVertexArray(0);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

#endif //PRAKTIKUM_3


#if PRAKTIKUM_3 == 1
    float speedAmplifier_backup = 0.0f;
    float speedAmplifier = 0.0f;
    bool resetAmplifier = false;

    float moonRotationSpeed = 0.05f;
    float planetRotationSpeed = 0.02f;

    float globalPlanetRotationSpeed = 0.01f;
#endif //PRAKTIKUM_3


#if PRAKTIKUM_3 == 1
    void initSystem() {

        sun.sphere.setN(3);
        sun.sphere.setRadius(0.5f);
        sun.setSphereColor(vec3(0.9f, 0.6f, 0.1f));
        sun.setBodyRotation(true);
        sun.lineVisible = true;
        sun.sphere.renderSphere();

        planet1.sphere.setN(3);
        planet1.sphere.setRadius(0.25f);
        planet1.sphere.absolutePosition = (vec3(-2.0f, 0.0f, 0.0f));
        planet1.setSphereColor(vec3(0.2f, 0.2f, 0.8f));
        planet1.setBodyRotation(true);
        planet1.lineVisible = true;
        planet1.sphere.renderSphere();

        moon1.sphere.setN(2);
        moon1.sphere.setRadius(0.1f);
        moon1.sphere.absolutePosition = (vec3(-2.5f, 0.0f, 0.0f));
        moon1.setSphereColor(vec3(0.4f, 0.8f, 0.9f));
        moon1.setBodyRotation(true);
        moon1.lineVisible = false;
        moon1.sphere.renderSphere();

        planet2.sphere.setN(3);
        planet2.sphere.setRadius(0.25f);
        planet2.sphere.absolutePosition = (vec3(2.0f, 0.0f, 0.0f));
        planet2.setSphereColor(vec3(0.2f, 0.2f, 0.8f));
        planet2.setBodyRotation(true);
        planet2.lineVisible = true;
        planet2.setInclinedStatus(true);
        planet2.sphere.renderSphere();

        moon2.sphere.setN(2);
        moon2.sphere.setRadius(0.1f);
        moon2.sphere.absolutePosition = (vec3(2.5f, 0.0f, 0.0f));
        moon2.setSphereColor(vec3(0.4f, 0.8f, 0.9f));
        moon2.setBodyRotation(true);
        moon2.lineVisible = false;
        moon2.sphere.renderSphere();

        moon1.setParentObject(&planet1);
        moon2.setParentObject(&planet2);
        
        planet1.childrenObjects.push_back(&moon1);
        planet1.setParentObject(&sun);
        planet2.childrenObjects.push_back(&moon2);
        planet2.setParentObject(&sun);

        sun.childrenObjects.push_back(&planet1);
        sun.childrenObjects.push_back(&planet2);

    }
#endif //PRAKTIKUM_3


#if DEFAULT == 1
void renderTriangle()
{
    // Create mvp.
    glm::mat4x4 mvp = projection * view * triangle.model;

    // Bind the shader program and set uniform(s).
    program.use();
    program.setUniform("mvp", mvp);

    // Bind vertex array object so we can render the 1 triangle.
    glBindVertexArray(triangle.vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void renderQuad()
{
    // Create mvp.
    glm::mat4x4 mvp = projection * view * quad.model;

    // Bind the shader program and set uniform(s).
    program.use();
    program.setUniform("mvp", mvp);

    // Bind vertex array object so we can render the 2 triangles.
    glBindVertexArray(quad.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
#endif

#if PRAKTIKUM_2 == 1
void renderSphere() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4x4 mvp = projection * view * sphereObject.model;

    program.use();
    program.setUniform("mvp", mvp);

    glBindVertexArray(sphereObject.vao);
    glDrawElements(GL_TRIANGLES, sphere.renderSphere().size() * 3, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    if (showCoords) {
        glBindVertexArray(sphereObject.linesVAO);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void renderNormals() {
    glm::mat4x4 mvp = projection * view * sphereNormalsObject.model;
    program.use();
    program.setUniform("mvp", mvp);

    glBindVertexArray(sphereNormalsObject.vao);
    glDrawArrays(GL_LINES, 0, sphere.generateNormalLines().size());
    glBindVertexArray(0);
}

#endif



#if DEFAULT == 1
void initTriangle()
{
    // Construct triangle. These vectors can go out of scope after we have send all data to the graphics card.
    const std::vector<glm::vec3> vertices = { glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) };
    const std::vector<glm::vec3> colors = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
    const std::vector<GLushort>  indices = { 0, 1, 2 };

    GLuint programId = program.getHandle();
    GLuint pos;

    // Step 0: Create vertex array object.
    glGenVertexArrays(1, &triangle.vao);
    glBindVertexArray(triangle.vao);

    // Step 1: Create vertex buffer object for position attribute and bind it to the associated "shader attribute".
    glGenBuffers(1, &triangle.positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangle.positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Bind it to position.
    pos = glGetAttribLocation(programId, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 2: Create vertex buffer object for color attribute and bind it to...
    glGenBuffers(1, &triangle.colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangle.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

    // Bind it to color.
    pos = glGetAttribLocation(programId, "color");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 3: Create vertex buffer object for indices. No binding needed here.
    glGenBuffers(1, &triangle.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

    // Unbind vertex array object (back to default).
    glBindVertexArray(0);

    // Modify model matrix.
    triangle.model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.25f, 0.0f, 0.0f));
}

std::vector<glm::vec3> QuadColors = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };
void initQuad()
{
    // Construct triangle. These vectors can go out of scope after we have send all data to the graphics card.
    const std::vector<glm::vec3> vertices = { { -1.0f, 1.0f, 0.0f }, { -1.0, -1.0, 0.0 }, { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } };
    //const std::vector<glm::vec3> colors   = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };
    const std::vector<GLushort>  indices = { 0, 1, 2, 0, 2, 3 };

    GLuint programId = program.getHandle();
    GLuint pos;

    // Step 0: Create vertex array object.
    glGenVertexArrays(1, &quad.vao);
    glBindVertexArray(quad.vao);

    // Step 1: Create vertex buffer object for position attribute and bind it to the associated "shader attribute".
    glGenBuffers(1, &quad.positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad.positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Bind it to position.
    pos = glGetAttribLocation(programId, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 2: Create vertex buffer object for color attribute and bind it to...
    glGenBuffers(1, &quad.colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, QuadColors.size() * sizeof(glm::vec3), QuadColors.data(), GL_STATIC_DRAW);

    // Bind it to color.
    pos = glGetAttribLocation(programId, "color");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 3: Create vertex buffer object for indices. No binding needed here.
    glGenBuffers(1, &quad.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

    // Unbind vertex array object (back to default).
    glBindVertexArray(0);

    // Modify model matrix.
    quad.model = glm::translate(glm::mat4(1.0f), glm::vec3(1.25f, 0.0f, 0.0f));
}
#endif


#if PRAKTIKUM_2 == 1
void initSphere() {
    auto& tris = sphere.renderSphere();
    sphere.transformRotation(sphere.getRotationMatrix());
    tris = sphere.getTriangles();

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<GLushort> indices;

    GLushort index = 0;
    for (const auto& tri : tris) {
        vertices.push_back(tri.v0);
        vertices.push_back(tri.v1);
        vertices.push_back(tri.v2);

        colors.push_back({ 1.0f, 1.0f, 0.0f });
        colors.push_back({ 1.0f, 1.0f, 0.0f });
        colors.push_back({ 1.0f, 1.0f, 0.0f });

        indices.push_back(index++);
        indices.push_back(index++);
        indices.push_back(index++);
    }

    GLuint programId = program.getHandle();
    GLuint pos;

    glGenVertexArrays(1, &sphereObject.vao);
    glBindVertexArray(sphereObject.vao);

    glGenBuffers(1, &sphereObject.positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereObject.positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    pos = glGetAttribLocation(programId, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &sphereObject.colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereObject.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
    pos = glGetAttribLocation(programId, "color");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &sphereObject.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereObject.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);


    std::vector<glm::vec3> coordLines = sphere.getCoords();
    std::vector<glm::vec3> coordColors = {
        {1,0,0}, {1,0,0}, // X-Achse (rot)
        {0,1,0}, {0,1,0}, // Y-Achse (grün)
        {0,0,1}, {0,0,1}  // Z-Achse (blau)
    };

    glGenVertexArrays(1, &sphereObject.linesVAO);
    glBindVertexArray(sphereObject.linesVAO);

    glGenBuffers(1, &sphereObject.linesPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereObject.linesPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, coordLines.size() * sizeof(glm::vec3), coordLines.data(), GL_STATIC_DRAW);
    pos = glGetAttribLocation(programId, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Farbpuffer
    glGenBuffers(1, &sphereObject.linesColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereObject.linesColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, coordColors.size() * sizeof(glm::vec3), coordColors.data(), GL_STATIC_DRAW);
    pos = glGetAttribLocation(programId, "color");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    sphereObject.model = glm::translate(glm::mat4(1.0f), sphere.absolutePosition);
}


void initNormals() {
    std::vector<glm::vec3> lines = sphere.generateNormalLines();
    GLuint programId = program.getHandle();
    GLuint pos;

    std::vector<glm::vec3> colors(lines.size(), glm::vec3(0.65f, 0.19f, 0.57f));

    glGenVertexArrays(1, &sphereNormalsObject.vao);
    glBindVertexArray(sphereNormalsObject.vao);

    glGenBuffers(1, &sphereNormalsObject.positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereNormalsObject.positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(glm::vec3), lines.data(), GL_STATIC_DRAW);
    pos = glGetAttribLocation(programId, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &sphereNormalsObject.colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereNormalsObject.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
    pos = glGetAttribLocation(programId, "color");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    sphereObject.model = glm::translate(glm::mat4(1.0f), sphere.absolutePosition);

}
#endif





/*
 Initialization. Should return true if everything is ok and false if something went wrong.
 */
bool init()
{
    // OpenGL: Set "background" color and enable depth testing.
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Construct view matrix.
    glm::vec3 eye(0.0f, 0.0f, 4.0f);
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    
    /*
    glm::vec3 eye(0.0f, 4.0f, 0.0f);    // Kamera über der Szene, auf Y=+4
    glm::vec3 center(0.0f, 0.0f, 0.0f); // Schaue zum Ursprung
    glm::vec3 up(0.0f, 0.0f, -1.0f);    // "Oben" zeigt jetzt in -Z Richtung
    */

    view = glm::lookAt(eye, center, up);

    // Create a shader program and set light direction.
    if (!program.compileShaderFromFile("shader/simple.vert", cg::GLSLShader::VERTEX)) {
        std::cerr << program.log();
        return false;
    }

    if (!program.compileShaderFromFile("shader/simple.frag", cg::GLSLShader::FRAGMENT)) {
        std::cerr << program.log();
        return false;
    }

    if (!program.link()) {
        std::cerr << program.log();
        return false;
    }

    // Create all objects.
#if DEFAULT == 1
    initTriangle();
    initQuad();
#endif

#if PRAKTIKUM_2 == 1
    initSphere();
    initNormals();
#endif

#if PRAKTIKUM_3 == 1
    sun.render();

    glmInit(sunBody, sun, true);
    glmInit(planet1Body, planet1, true);
    glmInit(moon1Body, moon1);
    glmInit(planet2Body, planet2, true);
    glmInit(moon2Body, moon2);

#endif //Praktikum_3
    return true;
}

/*
 Rendering.
 */
void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if DEFAULT == 1
    renderTriangle();
    renderQuad();
#endif

#if PRAKTIKUM_2 == 1
    renderSphere();
    if(showNormals)
        renderNormals();
#endif
#if PRAKTIKUM_3 == 1
    sun.render();

    glmInit(sunBody, sun, true);
    glmInit(planet1Body, planet1, true);
    glmInit(moon1Body, moon1);
    glmInit(planet2Body, planet2, true);
    glmInit(moon2Body, moon2);

    glmRender(sunBody, sun.sphere, true);
    glmRender(planet1Body, planet1.sphere, true);
    glmRender(moon1Body, moon1.sphere);
    glmRender(planet2Body, planet2.sphere, true);
    glmRender(moon2Body, moon2.sphere);
#endif
}

void glutDisplay()
{
    render();
    glutSwapBuffers();
}

/*
 Resize callback.
 */
void glutResize(int width, int height)
{
    // Division by zero is bad...
    height = height < 1 ? 1 : height;
    glViewport(0, 0, width, height);

    // Construct projection matrix.
    projection = glm::perspective(45.0f, (float)width / height, zNear, zFar);
}

/*
 Callback for char input.
 */
void glutKeyboard(unsigned char keycode, int x, int y)
{
    switch (keycode) {
    case 27: // ESC
        glutDestroyWindow(glutID);
        return;

#if PRAKTIKUM_2 == 1
        //tessellierung 
    case '+':
        sphere.increaseN();
        break;
    case '-':
        sphere.decreaseN();
        break;

        //Radius
    case 'r':
        sphere.decreaseRadius();
        break;
    case 'R':
        sphere.increaseRadius();
        break;

        //show Addons
    case 'c':
        showCoords = !showCoords;
        break;
    case 'v':
        showNormals = !showNormals;
        break;

        //Screne Zoom
    case 'a':
        sphere.zoomIn();
        break;
    case 's':
        sphere.zoomOut();
        break;

        //Object Rotation
    case 'n':
        sphere.resetRotation();
        break;
    case 'x':
        sphere.setXRotation();
        break;
    case 'y':
        sphere.setYRotation();
        break;
    case 'z':
        sphere.setZRotation();
        break;

#endif //PRAKTIKUM_2

#if PRAKTIKUM_3 == 1
    case '+':
        sun.sphere.increaseN();
        planet1.sphere.increaseN();
        moon1.sphere.increaseN();
        planet2.sphere.increaseN();
        moon2.sphere.increaseN();
        init();
        break;

    case '-':
        sun.sphere.decreaseN();
        planet1.sphere.decreaseN();
        moon1.sphere.decreaseN();
        planet2.sphere.decreaseN();
        moon2.sphere.decreaseN();
        init();
        break;

    case 'g':
        if (resetAmplifier) {
            resetAmplifier = false;
            speedAmplifier = speedAmplifier_backup;
        }
        else {
            speedAmplifier = 0;
            resetAmplifier = true;
        }
        break;
    case 'f':
        if (resetAmplifier) {
            resetAmplifier = false;
            speedAmplifier = speedAmplifier_backup;
        }
        else {
            speedAmplifier += 0.1;
            speedAmplifier_backup = speedAmplifier;
        }
        speedAmplifier += 0.1;
        break;
    case 'd':
        if (resetAmplifier) {
            resetAmplifier = false;
            speedAmplifier = speedAmplifier_backup;
        }
        else {
            speedAmplifier -= 0.1;
            speedAmplifier_backup = speedAmplifier;
        }
        break;
#endif //PRAKTIKUM_3

    }
#if PRAKTIKUM_2 == 1
    initSphere();
    initNormals();
#endif //PRAKTIKUM_2
    
    glutPostRedisplay();
}



#if PRAKTIKUM_3 == 1:
void animate(int value){
    planet1.sphere.setYRotation(planetRotationSpeed * speedAmplifier);
    planet2.sphere.setYRotation(planetRotationSpeed * speedAmplifier);
    moon1.sphere.setYRotation(moonRotationSpeed * speedAmplifier);
    moon2.sphere.setYRotation(moonRotationSpeed * speedAmplifier);

    planet1.yRotationValue = globalPlanetRotationSpeed * speedAmplifier;
    planet2.yRotationValue = globalPlanetRotationSpeed * speedAmplifier;
    moon1.yRotationValue = (globalPlanetRotationSpeed + 0.005) * speedAmplifier;
    moon2.yRotationValue = (globalPlanetRotationSpeed + 0.005) * speedAmplifier;
    sun.render();
    glutPostRedisplay();
    glutTimerFunc(16, animate, 0);
}
#endif //PRAKTIKUM_3


int main(int argc, char** argv)
{

#if PRAKTIKUM_1 == 1
    TranslateColorValues translator;

    //Aufgabe 1
    translator.handleInputTask01();

    //Aufgabe 2
    RGB rgb = translator.handleInputTask02();
    QuadColors = { {rgb.Red, rgb.Green, rgb.Blue}, {rgb.Red, rgb.Green, rgb.Blue}, {rgb.Red, rgb.Green, rgb.Blue}, {rgb.Red, rgb.Green, rgb.Blue} };
#endif // PRAKTIKUM_1



    // GLUT: Initialize freeglut library (window toolkit).
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(40, 40);
    glutInit(&argc, argv);

    // GLUT: Create a window and opengl context (version 4.3 core profile).
    glutInitContextVersion(4, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);



    glutCreateWindow("Aufgabenblatt 01");
    glutID = glutGetWindow();

    // GLEW: Load opengl extensions
    //glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }

#if PRAKTIKUM_3 == 1
    initSystem();
#endif


#if _DEBUG
    if (glDebugMessageCallback) {
        std::cout << "Register OpenGL debug callback " << std::endl;
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(cg::glErrorVerboseCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            nullptr,
            true); // get all debug messages
    }
    else {
        std::cout << "glDebugMessageCallback not available" << std::endl;
    }
#endif

    // GLUT: Set callbacks for events.
    glutReshapeFunc(glutResize);
    glutDisplayFunc(glutDisplay);
    //glutIdleFunc   (glutDisplay); // redisplay when idle

#if PRAKTIKUM_3 == 1:
    glutTimerFunc(16, animate, 0);
#endif //PRAKTIKUM_3

    glutKeyboardFunc(glutKeyboard);

    // init vertex-array-objects.
    bool result = init();
    if (!result) {
        return -2;
    }

    // GLUT: Loop until the user closes the window
    // rendering & event handling
    glutMainLoop();

    // Cleanup in destructors:
    // Objects will be released in ~Object
    // Shader program will be released in ~GLSLProgram

    return 0;
}










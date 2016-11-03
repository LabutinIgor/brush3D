#ifndef OBJVIEWER_MAINVIEW_H
#define OBJVIEWER_MAINVIEW_H

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>

#include "load_shaders.h"
#include "MainView.h"

class MainView {
public:
    MainView(const char *objFileName, const char *textureFileName);

    void show();

private:
    void initGlfwWindow();

    void initGlew();

    void loadObj(const char *fileName);

    void loadTexture(const char *fileName);

    void initMVPMatrices();

    void setMouseCallbacks();

    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

    void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    void draw();

    void updateRotation(double currentMousePositionX, double currentMousePositionY);

    const char *objFileName;
    const char *textureFileName;
    GLFWwindow *window;
    GLuint programID;
    GLint matrixID;
    GLuint verticesID;
    GLuint texture;
    GLint textureID;
    GLuint vertexBuffer;
    GLuint uvBuffer;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvCoordinates;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 scaleMatrix;
    glm::mat4 rotationMatrix;
    double scaleCoefficient = 1;
    bool mousePressed = false;
    double previousMousePositionX;
    double previousMousePositionY;

};


#endif //OBJVIEWER_MAINVIEW_H

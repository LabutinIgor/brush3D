#include "SOIL.h"
#include "MainView.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"


MainView::MainView(const char *objFileName, const char *textureFileName) :
        objFileName(objFileName), textureFileName(textureFileName) {}

void MainView::show() {
    initGlfwWindow();
    initGlew();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &verticesID);
    glBindVertexArray(verticesID);

    programID = loadShaders("../resources/vertex_shader.vert", "../resources/fragment_shader.frag");
    matrixID = glGetUniformLocation(programID, "matrixMVP");
    loadObj(objFileName);
    loadTexture("../resources/img_test.png");

    initMVPMatrices();
    setMouseCallbacks();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvCoordinates.size(), &uvCoordinates[0], GL_STATIC_DRAW);

    while (glfwWindowShouldClose(window) == 0) {
        draw();
    }

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &verticesID);
    glfwTerminate();
}

void MainView::initGlfwWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        exit(1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "ObjViewer", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to open GLFW window.\n";
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
}

void MainView::initGlew() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        exit(1);
    }
}

void MainView::loadObj(const char *fileName) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName);
    if (!err.empty()) {
        std::cerr << err << std::endl;
    }
    if (!ret) {
        exit(1);
    }

    vertices.clear();
    uvCoordinates.clear();
    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                float vx = attrib.vertices[3 * idx.vertex_index + 0];
                float vy = attrib.vertices[3 * idx.vertex_index + 1];
                float vz = attrib.vertices[3 * idx.vertex_index + 2];
                float nx = attrib.normals[3 * idx.normal_index + 0];
                float ny = attrib.normals[3 * idx.normal_index + 1];
                float nz = attrib.normals[3 * idx.normal_index + 2];
                float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                vertices.push_back(glm::vec3(vx, vy, vz));
                uvCoordinates.push_back(glm::vec2(tx, ty));
            }
            index_offset += fv;
            shapes[s].mesh.material_ids[f];
        }
    }
}

void MainView::loadTexture(const char *fileName) {
    int width, height;
    unsigned char *image =
            SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGB);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    textureID = glGetUniformLocation(programID, "textureSampler");
}

void MainView::initMVPMatrices() {
    projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    viewMatrix = glm::lookAt(
            glm::vec3(0, 0, 10),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );
    rotationMatrix = glm::mat4(1.f);
    scaleMatrix = glm::mat4(1.f);
}

void MainView::setMouseCallbacks() {
    glfwSetWindowUserPointer(window, this);
    auto buttonCallback = [](GLFWwindow *window, int button, int action, int mods) {
        static_cast<MainView *>(glfwGetWindowUserPointer(window))->mouseButtonCallback(window, button, action, mods);
    };
    glfwSetMouseButtonCallback(window, buttonCallback);

    auto scrollCallback = [](GLFWwindow *window, double xoffset, double yoffset) {
        static_cast<MainView *>(glfwGetWindowUserPointer(window))->scrollCallback(window, xoffset, yoffset);
    };
    glfwSetScrollCallback(window, scrollCallback);

}

void MainView::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(window, &previousMousePositionX, &previousMousePositionY);
            mousePressed = true;
        } else {
            if (action == GLFW_RELEASE) {
                mousePressed = false;
            }
        }
    }
}

void MainView::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    scaleCoefficient += yoffset * 0.1;
}

void MainView::updateRotation(double currentMousePositionX, double currentMousePositionY) {
    double dx = previousMousePositionX - currentMousePositionX;
    double dy = previousMousePositionY - currentMousePositionY;

    double r = sqrt(dx * dx + dy * dy);
    rotationMatrix = glm::eulerAngleXYZ((float) (dy * r * 0.001f), (float) (dx * r * 0.001f), 0.0f) * rotationMatrix;

    previousMousePositionX = currentMousePositionX;
    previousMousePositionY = currentMousePositionY;
}

void MainView::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);

    if (mousePressed) {
        double currentMousePositionX, currentMousePositionY;
        glfwGetCursorPos(window, &currentMousePositionX, &currentMousePositionY);
        updateRotation(currentMousePositionX, currentMousePositionY);
    }

    scaleMatrix = glm::scale(glm::mat4(1.f), glm::tvec3<float>(scaleCoefficient, scaleCoefficient, scaleCoefficient));
    glm::mat4 matrixMVP = projectionMatrix * viewMatrix * rotationMatrix * scaleMatrix;

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrixMVP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureID, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

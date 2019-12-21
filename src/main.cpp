#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "Shader.hpp"
#include "Cube.hpp"

const unsigned int WIDTH = 512;
const unsigned int HEIGHT = 512;

bool detectCollision(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 dim1, glm::vec3 dim2);

int main() {
    // initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "voxel", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    Shader shader("src/shader.vert", "src/shader.frag");

    // loop
    Cube cube = Cube();
    const glm::vec3 red = glm::vec3(1.0, 0.0, 0.0);
    const glm::vec3 green = glm::vec3(0.0, 1.0, 0.0);
    const glm::vec3 blue = glm::vec3(0.0, 0.0, 1.0);
    glm::vec3 face_colors[6] = {red, red, green, green, blue, blue};
    cube.setFaceColors(face_colors);
    cube.updateCube();

    glEnable(GL_DEPTH_TEST);
    float degrees = 0.0;
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i=-3; i<=3;i++) {
            for (int j =-3;j<=3;j++) {
                glm::mat4 model = glm::mat4(1.0);
                model = glm::translate(model, glm::vec3((float)2*i, (float)2*j, 0.0));
                model = glm::scale(model, glm::vec3(0.50, 0.50, 0.50));
                glm::mat4 view = glm::mat4(1.0);
                int state = glfwGetKey(window, GLFW_KEY_RIGHT);
                if (state == GLFW_PRESS) {degrees += 0.001;}
                state = glfwGetKey(window, GLFW_KEY_LEFT);
                if (state == GLFW_PRESS) {degrees -= 0.001;}
                view = glm::rotate(view, glm::radians(degrees), glm::vec3(0.0, 1.0, 0.0));
                view = glm::translate(view, glm::vec3(0.0, -2.0, -30.0));
                /*int state = glfwGetKey(window, GLFW_KEY_RIGHT);
                if (state == GLFW_PRESS) {degrees += 0.01;}
                state = glfwGetKey(window, GLFW_KEY_LEFT);
                if (state == GLFW_PRESS) {degrees -= 0.01;}
                view = glm::rotate(view, glm::radians(degrees), glm::vec3(0.0, 1.0, 0.0));*/
                glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
                shader.use();
                shader.setMat4("model", model);
                shader.setMat4("view", view); 
                shader.setMat4("projection", projection);
                cube.draw();
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << detectCollision(glm::vec3(0,0,0), glm::vec3(1,1,1), glm::vec3(1,1,1), glm::vec3(1,1,1)) << std::endl;

    glfwTerminate();
    return 0;
}

bool detectCollision(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 dim1, glm::vec3 dim2) {
    bool xIntersect = (pos1.x - dim1.x/2) < (pos2.x + dim2.x/2) && (pos1.x + dim1.x/2) > (pos2.x - dim2.x/2);
    bool yIntersect = (pos1.y - dim1.y/2) < (pos2.y + dim2.y/2) && (pos1.y + dim1.y/2) > (pos2.y - dim2.y/2);
    bool zIntersect = (pos1.z - dim1.z/2) < (pos2.x + dim2.z/2) && (pos1.z + dim1.z/2) > (pos2.z - dim2.z/2);
    return xIntersect && yIntersect && zIntersect;
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "headers/Shader.hpp"
#include "headers/Cube.hpp"
#include "headers/Matrix.hpp"

const unsigned int WIDTH = 512;
const unsigned int HEIGHT = 512;

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
    Shader shader("src/shaders/shader.vert", "src/shaders/shader.frag");

    // loop
    Cube cube = Cube();
    const mtx::vec3 red = mtx::vec3(1.0, 0.0, 0.0);
    const mtx::vec3 green = mtx::vec3(0.0, 1.0, 0.0);
    const mtx::vec3 blue = mtx::vec3(0.0, 0.0, 1.0);
    mtx::vec3 face_colors[6] = {red, red, green, green, blue, blue};
    cube.setFaceColors(face_colors);
    cube.updateCube();

    mtx::fmt = mtx::COL;
    glEnable(GL_DEPTH_TEST);
    float degrees = 0.0;
    float dist = 0.0;
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i=-3; i<=3;i++) {
            for (int j =-3;j<=3;j++) {
                
                mtx::mat4 model;
                mtx::identity(model);
                mtx::translate(model, mtx::vec3((float)2*i, (float)2*j, 0.0));
                mtx::scale(model, mtx::vec3(0.50, 0.50, 0.50));
                
                mtx::mat4 view;
                mtx::identity(view);
                int state = glfwGetKey(window, GLFW_KEY_RIGHT);
                if (state == GLFW_PRESS) {degrees += 0.01;}
                state = glfwGetKey(window, GLFW_KEY_LEFT);
                if (state == GLFW_PRESS) {degrees -= 0.01;}
                state = glfwGetKey(window, GLFW_KEY_UP);
                if (state == GLFW_PRESS) {dist += 0.001;}
                state = glfwGetKey(window, GLFW_KEY_DOWN);
                if (state == GLFW_PRESS) {dist -= 0.001;}
                mtx::translate(view, mtx::vec3(0.0, -2.0, -30.0+dist));
                mtx::rotate(view, mtx::radians(degrees), mtx::vec3(0.0, 1.0, 0.0));

                mtx::mat4 projection = mtx::perspective(mtx::radians(45.0f), (float)HEIGHT/(float)WIDTH, 0.1f, 100.0f);

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
    
    glfwTerminate();
    return 0;
}

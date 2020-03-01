#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "headers/Shader.hpp"
#include "headers/Matrix.hpp"
#include "headers/stb_image.h"
#include "headers/Chunk.hpp"

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
    
    ChunkManager<16,16,16> chunkmanager(
            10, 4, // number of chunks, chunks on x axis
            0, 0, -30, // coords of world start
            "src/resources/grass.png"); // texture for blocks

    mtx::fmt = mtx::COL;
    glEnable(GL_DEPTH_TEST);
    float degrees = 0.0;
    float x = 0;
    float y = 0;
    float z = -30;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mtx::mat4 view;
        mtx::identity(view);
        int state = glfwGetKey(window, GLFW_KEY_RIGHT);
        if (state == GLFW_PRESS) {degrees += 0.2;}
        state = glfwGetKey(window, GLFW_KEY_LEFT);
        if (state == GLFW_PRESS) {degrees -= 0.2;}
        state = glfwGetKey(window, GLFW_KEY_UP);
        if (state == GLFW_PRESS) {
            float step = 0.1;
            float dx = step*cos(3.14159*(degrees+90)/180.0);
            float dz = step*sin(3.14159*(degrees+90)/180.0);
            x += dx;
            z += dz;
        }
        state = glfwGetKey(window, GLFW_KEY_S);
        if (state == GLFW_PRESS) {
            float step = 0.1;
            y += step;
        }
        state = glfwGetKey(window, GLFW_KEY_W);
        if (state == GLFW_PRESS) {
            float step = 0.1;
            y -= step;
        }

        mtx::translate(view, mtx::vec3(x, y, z));
        mtx::rotate(view, mtx::radians(degrees), mtx::vec3(0.0, 1.0, 0.0));
        
        mtx::mat4 projection = mtx::perspective(mtx::radians(45.0f), (float)HEIGHT/(float)WIDTH, 0.1f, 100.0f);

        shader.use();
        shader.setMat4("view", view); 
        shader.setMat4("projection", projection);
        
        chunkmanager.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

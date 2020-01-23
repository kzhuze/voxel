#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include "Matrix.hpp"

class Cube {
    public:
        Cube() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
        }
        void setFaceColors(mtx::vec3 colors[6]) {
            for (int i = 0; i < 6; i++) {
                for (int j = 36*i+3; j < 36*(i+1); j += 6) {
                    mtx::vec3 color = colors[i];
                    vertices[j] = color[0];
                    vertices[j+1] = color[1];
                    vertices[j+2] = color[2];
                }
            }
        }
        void updateCube() {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }
        void draw() {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    private:
        unsigned int VAO;
        unsigned int VBO;
        float vertices[36 * 6] = { 
            -1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0, 
             1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0, 
             1.0f,  1.0f, -1.0f, 0.0, 0.0, 0.0, 
             1.0f,  1.0f, -1.0f, 0.0, 0.0, 0.0, 
            -1.0f,  1.0f, -1.0f, 0.0, 0.0, 0.0, 
            -1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0, 

            -1.0f, -1.0f,  1.0f, 0.0, 0.0, 0.0,  
             1.0f, -1.0f,  1.0f, 0.0, 0.0, 0.0, 
             1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0, 
             1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0, 
            -1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0, 
            -1.0f, -1.0f,  1.0f, 0.0, 0.0, 0.0, 

            -1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0,
            -1.0f,  1.0f, -1.0f, 0.0, 0.0, 0.0,
            -1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0,
            -1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0,
            -1.0f, -1.0f,  1.0f, 0.0, 0.0, 0.0,
            -1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0,

             1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0,
             1.0f,  1.0f, -1.0f, 0.0, 0.0, 0.0,
             1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0,
             1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0,
             1.0f, -1.0f,  1.0f, 0.0, 0.0, 0.0,
             1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0,

            -1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0,
             1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0,
             1.0f, -1.0f,  1.0f, 0.0, 0.0, 0.0,
             1.0f, -1.0f,  1.0f, 0.0, 0.0, 0.0,
            -1.0f, -1.0f,  1.0f, 0.0, 0.0, 0.0,
            -1.0f, -1.0f, -1.0f, 0.0, 0.0, 0.0,

            -1.0f,  1.0f, -1.0f, 0.0, 0.0, 0.0,
             1.0f,  1.0f, -1.0f, 0.0, 0.0, 0.0,
             1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0,
             1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0,
            -1.0f,  1.0f,  1.0f, 0.0, 0.0, 0.0,
            -1.0f,  1.0f, -1.0f, 0.0, 0.0, 0.0 
        };
};
#endif

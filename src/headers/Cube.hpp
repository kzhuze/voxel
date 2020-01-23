#pragma once

#include <glad/glad.h>
#include "Matrix.hpp"

class Cube {
    public:
        Cube() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &vertexVBO);
            glGenBuffers(1, &colorVBO);
        }
        void setFaceColors(mtx::vec3 _colors[6]) {
            int c = -1;
            for (int i = 0; i < 36*3; i+=3) {
                if (i%(36*3/6)==0) { c++; }
                colors[i]   = _colors[c][0];
                colors[i+1] = _colors[c][1];
                colors[i+2] = _colors[c][2];
            }
        }
        void updateCube() {
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (void*)0);
            
            glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STREAM_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (void*)0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }
        void draw() {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    private:
        unsigned int VAO;
        unsigned int colorVBO;
        float colors[36 * 3] = {};
        unsigned int vertexVBO;
        float vertices[36 * 3] = { 
            -1.0f, -1.0f, -1.0f, 
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f, 
             1.0f,  1.0f, -1.0f, 
            -1.0f,  1.0f, -1.0f, 
            -1.0f, -1.0f, -1.0f, 

            -1.0f, -1.0f,  1.0f,  
             1.0f, -1.0f,  1.0f, 
             1.0f,  1.0f,  1.0f, 
             1.0f,  1.0f,  1.0f, 
            -1.0f,  1.0f,  1.0f, 
            -1.0f, -1.0f,  1.0f, 

            -1.0f,  1.0f,  1.0f, 
            -1.0f,  1.0f, -1.0f, 
            -1.0f, -1.0f, -1.0f, 
            -1.0f, -1.0f, -1.0f, 
            -1.0f, -1.0f,  1.0f, 
            -1.0f,  1.0f,  1.0f, 

             1.0f,  1.0f,  1.0f, 
             1.0f,  1.0f, -1.0f, 
             1.0f, -1.0f, -1.0f, 
             1.0f, -1.0f, -1.0f, 
             1.0f, -1.0f,  1.0f, 
             1.0f,  1.0f,  1.0f, 

            -1.0f, -1.0f, -1.0f, 
             1.0f, -1.0f, -1.0f, 
             1.0f, -1.0f,  1.0f, 
             1.0f, -1.0f,  1.0f, 
            -1.0f, -1.0f,  1.0f, 
            -1.0f, -1.0f, -1.0f, 

            -1.0f,  1.0f, -1.0f, 
             1.0f,  1.0f, -1.0f, 
             1.0f,  1.0f,  1.0f, 
             1.0f,  1.0f,  1.0f, 
            -1.0f,  1.0f,  1.0f, 
            -1.0f,  1.0f, -1.0f 
        };
};

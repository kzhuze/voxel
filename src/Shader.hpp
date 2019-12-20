#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        unsigned int ID;
        Shader(std::string vertexPath, std::string fragmentPath) {
            std::string vertexString, fragmentString;
            vertexString = readShaderFile(vertexPath, "VERTEX");
            fragmentString = readShaderFile(fragmentPath, "FRAGMENT");
            const char *vertexCode = vertexString.c_str();
            const char *fragmentCode = fragmentString.c_str();

            unsigned int vertexShader, fragmentShader;
            
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexCode, NULL);
            glCompileShader(vertexShader);
            checkCompileErrors(vertexShader, "VERTEX");

            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
            glCompileShader(fragmentShader);
            checkCompileErrors(fragmentShader, "FRAGMENT");

            ID = glCreateProgram();
            glAttachShader(ID, vertexShader);
            glAttachShader(ID, fragmentShader);
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }
        void use() { 
            glUseProgram(ID); 
        }
        void setInt(const std::string &name, int value) const { 
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
        }
        void setFloat(const std::string &name, float value) const { 
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
        }
        void setVec4(const std::string &name, float val1, float val2, float val3, float val4) const {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), val1, val2, val3, val4);
        }
        void setMat4(const std::string &name, glm::mat4 matrix) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
        }
    private:
        std::string readShaderFile(std::string filePath, std::string type) {
            std::ifstream shaderFile(filePath);
            std::string shaderString;
            if (shaderFile) {
                std::ostringstream ss;
                ss << shaderFile.rdbuf();
                shaderString = ss.str();
            } else {
                std::cout << "ERROR::SHADER::" << type << "::INVALD_FILE_PATH\n";
            }
            return shaderString;
        }
        void checkCompileErrors(unsigned int shader, std::string type) {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog;
                }
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
                }
            }
        }
};
#endif

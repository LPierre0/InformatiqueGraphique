#ifndef QUAD_H
#define QUAD_H

#include "../include.h"

class Quad {
public:
    Quad(GLuint texture, Shader& shader);
    void render();

private:
    GLuint VAO, VBO;
    GLuint texture;
    Shader shader;
};

#endif
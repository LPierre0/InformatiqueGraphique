#include "Quad.h"


Quad::Quad(GLuint texture, Shader& shader) : shader(shader) {
    this->texture = texture;
    this->shader = shader;

    float quadVertices[] = {
        // Positions           // Normales         // TexCoords
        -1.0f,  1.0f, -10.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, -10.0f,   0.0f,  1.0f,  0.0f,   0.0f,  0.0f,
         1.0f, -1.0f, -10.0f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,

        -1.0f,  1.0f,  -10.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         1.0f, -1.0f,  -10.0f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  -10.0f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    
    // Attribut 0 : Positions (3 floats)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // Attribut 1 : Normales (3 floats)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // Attribut 2 : Coordonnées de texture (2 floats)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    glBindVertexArray(0);
}


void Quad::render(){

    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

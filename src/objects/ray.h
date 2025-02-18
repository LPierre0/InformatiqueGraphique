#ifndef RAY_H
#define RAY_H

#include "../include.h"


class Ray {
public:

    glm::vec3 origin; 
    glm::vec3 end;
    glm::vec3 direction; 

    double mouseX, mouseY;

    unsigned int VAO, VBO;

    void calculateDirection(int screenWidht, int screenHeight, glm::mat4 projectionMatrix, glm::mat4 view);
    void calculateEnd();
    void draw();
    void update(int screenWidht, int screenHeight, glm::mat4 projectionMatrix, glm::mat4 view, const glm::vec3& o, double mX, double mY);
};

#endif


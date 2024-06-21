#ifndef SPHERE_H
#define SPHERE_H

#include "include.h"
#include "ray.h"

class Sphere
{
public:

    Sphere(glm::vec3 center, float radius, int sectorCount, int stackCount);
    ~Sphere() {}
    unsigned int VAO, VBO, EBO;
    glm::vec3 center; 
    float radius;
    int sectorCount, stackCount;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> final;

    std::vector<int> indices;
    std::vector<int> normalIndices;

    void set(glm::vec3 center, float radius, int sectorCount, int stackCount);
    void calculatePointsAndNormals();
    void calculateIndices();
    void finalData();
    void update(float radius, int sectorCount, int stackCount);
    void draw();
    bool intersect(Ray r);
    void drawIntersection(Ray rayon);
    float t;

};

#endif

#ifndef SPHERE_H
#define SPHERE_H

#include "../include.h"
#include "../core/Object.h"

class Sphere : public Object
{
    public:
        Sphere(GLuint texture, glm::vec3 center, float radius, int sectorCount, int stackCount);
        ~Sphere() {}

        void compute_points_and_normals();
        void compute_texture_coordinates();
        void compute_indices();

    private:
        float radius;
        int sector_count, stack_count;


};

#endif

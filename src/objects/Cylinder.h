#ifndef CYLINDER_H
#define CYLINDER_H

#include "../include.h"
#include "../core/Object.h"

class Cylinder : public Object
{
    public:
        Cylinder(GLuint texture, glm::vec3 center, glm::vec3 color, glm::vec3 axis, float radius, float length, int subdiv1, int subdiv2);
        ~Cylinder() {}

        void compute_points_and_normals();
        void compute_texture_coordinates();
        void compute_indices();
        void update_mesh() override;

    private:
        float radius;
        float length;
        int subdiv1;
        int subdiv2;
        glm::vec3 base;
        glm::vec3 axis;
};

#endif

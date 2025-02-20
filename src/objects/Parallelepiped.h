#ifndef PARALLELEPIPED_H
#define PARALLELEPIPED_H

#include "../include.h"
#include "../core/Object.h"
#include "../utils.h"


class Parallelepiped : public Object {
    public:
        Parallelepiped(GLuint texture, glm::vec3 center, float width, float height, float depth);


        void calculate_points();

        void calculate_normals();

        void calculate_normals_inv();

        void calculate_texture_coordinates();

    private:

        float width, height, depth;


};


#endif

#ifndef PARALLELEPIPED_H
#define PARALLELEPIPED_H

#include "../include.h"
#include "../core/Object.h"


class Parallelepiped : public Object 
{
    public:
        Parallelepiped(GLuint texture, glm::vec3 center, float width, float height, float depth);


        void compute_points();

        void compute_normals();

        void compute_texture_coordinates();

    private:

        float width, height, depth;


};


#endif

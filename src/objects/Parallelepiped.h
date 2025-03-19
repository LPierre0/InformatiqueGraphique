#ifndef PARALLELEPIPED_H
#define PARALLELEPIPED_H

#include "../include.h"
#include "../core/Object.h"


class Parallelepiped : public Object 
{
    public:
        Parallelepiped(GLuint texture, glm::vec3 center, glm::vec3 color, float width, float height, float depth);


        void compute_points() override;

        void compute_normals() override;

        void compute_indices() override;
        void compute_texture_coordinates();

    private:

        float width, height, depth;


};


#endif

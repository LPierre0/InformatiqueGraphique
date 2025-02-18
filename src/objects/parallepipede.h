#ifndef PARAL_H
#define PARAL_H

#include "../include.h"

class PARALLEPIPEDE{
    public:
        unsigned int VAO, VBO, EBO;
        PARALLEPIPEDE(std::vector<glm::vec3> points, std::vector<std::vector<int>> faces, glm::vec3 center, GLuint texture);
        ~PARALLEPIPEDE() {}
        glm::vec3 center;
        GLuint texture;
        // Les points vont correspondre aux coordonnées des points par rapport aux centre
        // Les faces vont correspondre aux indices des points pour chaque face : {1, 2, 3, 2, 4, 3} par exemple pour la face 1.
        std::vector<glm::vec3> points;
        std::vector<std::vector<int>> faces;
        std::vector<int> indices;
        std::vector<glm::vec3> final;
        std::vector<glm::vec3> normals;

        void calculateNormals();
        void draw();
        void getFinalData();
        void calculateIndices();
};

#endif


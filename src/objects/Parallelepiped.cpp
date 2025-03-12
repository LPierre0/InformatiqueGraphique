#include "Parallelepiped.h"




Parallelepiped::Parallelepiped(GLuint texture, glm::vec3 center, float width, float height, float depth)
    : Object(texture, center){
        this->width = width;
        this->height = height;
        this->depth = depth;

        this->compute_points();
        this->compute_normals();
        this->compute_texture_coordinates();
        this->compute_final();
        this->update();
}



void Parallelepiped::compute_points(){
    this->points.clear();
    glm::vec3 p1 = center + glm::vec3(-width / 2, -height / 2, -depth / 2);
    glm::vec3 p2 = center + glm::vec3(width / 2, -height / 2, -depth / 2);
    glm::vec3 p3 = center + glm::vec3(width / 2, height / 2, -depth / 2);
    glm::vec3 p4 = center + glm::vec3(-width / 2, height / 2, -depth / 2);
    glm::vec3 p5 = center + glm::vec3(-width / 2, -height / 2, depth / 2);
    glm::vec3 p6 = center + glm::vec3(width / 2, -height / 2, depth / 2);
    glm::vec3 p7 = center + glm::vec3(width / 2, height / 2, depth / 2);
    glm::vec3 p8 = center + glm::vec3(-width / 2, height / 2, depth / 2);


    points = {
        p1, p2, p3, p4,  // Face avant
        p6, p5, p8, p7,  // Face arrière
        p1, p2, p6, p5,  // Face inférieure
        p4, p3, p7, p8,  // Face supérieure
        p5, p1, p4, p8,  // Face gauche
        p2, p6, p7, p3   // Face droite
    };

    // Indices pour former les triangles (chaque face est indépendante)
    indices = {
        1, 0, 2, 2, 0, 3,    // Face avant
        5, 4, 6, 6, 4, 7,    // Face arrière
        8, 9, 10, 8, 10, 11, // Face inférieure
        13, 12, 14, 14, 12, 15, // Face supérieure
        17, 16, 18, 18, 16, 19, // Face gauche
        21, 20, 22, 22, 20, 23  // Face droite
    };
}



void Parallelepiped::compute_normals(){
    this->normals.clear();
    int nb_points = indices.size();

    std::vector<float> nb_norm(nb_points, 0.0f);
    std::vector<glm::vec3> norm_to_treat(nb_points, {0.0f, 0.0f, 0.0f});

    for (int i = 0; i < nb_points/3; i++){
        int ind_p1 = indices[i * 3 + 0];
        int ind_p2 = indices[i * 3 + 1];
        int ind_p3 = indices[i * 3 + 2];
        glm::vec3 p1 = points[ind_p1];
        glm::vec3 p2 = points[ind_p2];
        glm::vec3 p3 = points[ind_p3];

        glm::vec3 normal = compute_normal(p1, p2, p3);

        norm_to_treat[ind_p1] += normal;

        norm_to_treat[ind_p2] += normal;

        norm_to_treat[ind_p3] += normal;
    }


    for (int i = 0; i < nb_points; i++){
        normals.push_back(glm::normalize(norm_to_treat[i]));
    }
    
}


void Parallelepiped::compute_texture_coordinates(){
    this->texCoords.clear();

    // Face Avant
    texCoords.push_back({1.0f * width, 1.0f * height});
    texCoords.push_back({0.0f * width, 1.0f * height});
    texCoords.push_back({0.0f * width, 0.0f * height});
    texCoords.push_back({1.0f * width, 0.0f * height});

    // Face Arriere
    texCoords.push_back({1.0f * width, 1.0f * height});
    texCoords.push_back({0.0f * width, 1.0f * height});
    texCoords.push_back({0.0f * width, 0.0f * height});
    texCoords.push_back({1.0f * width, 0.0f * height});


    // Face inf
    texCoords.push_back({1.0f * width, 1.0f * depth});
    texCoords.push_back({0.0f * width, 1.0f * depth});
    texCoords.push_back({0.0f * width, 0.0f * depth});
    texCoords.push_back({1.0f * width, 0.0f * depth});

    // Face sup
    texCoords.push_back({1.0f * width, 1.0f * depth});
    texCoords.push_back({0.0f * width, 1.0f * depth});
    texCoords.push_back({0.0f * width, 0.0f * depth});
    texCoords.push_back({1.0f * width, 0.0f * depth});

    // Face Gauche
    texCoords.push_back({1.0f * depth, 1.0f * height});
    texCoords.push_back({0.0f * depth, 1.0f * height});
    texCoords.push_back({0.0f * depth, 0.0f * height});
    texCoords.push_back({1.0f * depth, 0.0f * height});

    // Face droite
    texCoords.push_back({1.0f * depth, 1.0f * height});
    texCoords.push_back({0.0f * depth, 1.0f * height});
    texCoords.push_back({0.0f * depth, 0.0f * height});
    texCoords.push_back({1.0f * depth, 0.0f * height});

}




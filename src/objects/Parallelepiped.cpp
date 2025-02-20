#include "Parallelepiped.h"




Parallelepiped::Parallelepiped(GLuint texture, glm::vec3 center, float width, float height, float depth)
    : Object(texture, center){
        this->width = width;
        this->height = height;
        this->depth = depth;

        this->calculate_points();
        this->calculate_normals();
        this->calculate_texture_coordinates();
        this->calculate_final();
}



void Parallelepiped::calculate_points(){
    // Définition des 8 sommets de base
    glm::vec3 p1 = center + glm::vec3(-width / 2, -height / 2, -depth / 2);
    glm::vec3 p2 = center + glm::vec3(width / 2, -height / 2, -depth / 2);
    glm::vec3 p3 = center + glm::vec3(width / 2, height / 2, -depth / 2);
    glm::vec3 p4 = center + glm::vec3(-width / 2, height / 2, -depth / 2);
    glm::vec3 p5 = center + glm::vec3(-width / 2, -height / 2, depth / 2);
    glm::vec3 p6 = center + glm::vec3(width / 2, -height / 2, depth / 2);
    glm::vec3 p7 = center + glm::vec3(width / 2, height / 2, depth / 2);
    glm::vec3 p8 = center + glm::vec3(-width / 2, height / 2, depth / 2);


    points.push_back(p1); 
    points.push_back(p2); 
    points.push_back(p3); 
    points.push_back(p4); 

    points.push_back(p6); 
    points.push_back(p5); 
    points.push_back(p8); 
    points.push_back(p7); 

    points.push_back(p5); 
    points.push_back(p6); 
    points.push_back(p2); 
    points.push_back(p1); 

    points.push_back(p4); 
    points.push_back(p3); 
    points.push_back(p7); 
    points.push_back(p8); 

    points.push_back(p5); 
    points.push_back(p1); 
    points.push_back(p4); 
    points.push_back(p8); 

    points.push_back(p2); 
    points.push_back(p6); 
    points.push_back(p7); 
    points.push_back(p3); 

   

    // Indices pour former les triangles
    indices = {
        0, 1, 2, 0, 2, 3,   // Face avant
        4, 5, 6, 4, 6, 7, // Face arrière
        8, 9, 10, 8, 10, 11, // Face inférieure
        12, 13, 14, 12, 14, 15, // Face supérieure
        16, 17, 18, 16, 18, 19, // Face gauche
        20, 21, 22, 20, 22, 23  // Face droite
    };
}



void Parallelepiped::calculate_normals(){
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
        std::cout << norm_to_treat[i][0] << norm_to_treat[i][1] << norm_to_treat[i][2] << std::endl;
    }
    
}


void Parallelepiped::calculate_texture_coordinates(){
    // Face avant
    texCoords.push_back({1.0f, 1.0f});
    texCoords.push_back({0.0f, 1.0f});
    texCoords.push_back({0.0f, 0.0f});
    texCoords.push_back({1.0f, 0.0f});

    texCoords.push_back({1.0f, 1.0f});
    texCoords.push_back({0.0f, 1.0f});
    texCoords.push_back({0.0f, 0.0f});
    texCoords.push_back({1.0f, 0.0f});

    texCoords.push_back({1.0f, 1.0f});
    texCoords.push_back({0.0f, 1.0f});
    texCoords.push_back({0.0f, 0.0f});
    texCoords.push_back({1.0f, 0.0f});

    texCoords.push_back({1.0f, 1.0f});
    texCoords.push_back({0.0f, 1.0f});
    texCoords.push_back({0.0f, 0.0f});
    texCoords.push_back({1.0f, 0.0f});

    texCoords.push_back({1.0f, 1.0f});
    texCoords.push_back({0.0f, 1.0f});
    texCoords.push_back({0.0f, 0.0f});
    texCoords.push_back({1.0f, 0.0f});

    texCoords.push_back({1.0f, 1.0f});
    texCoords.push_back({0.0f, 1.0f});
    texCoords.push_back({0.0f, 0.0f});
    texCoords.push_back({1.0f, 0.0f});

    
}




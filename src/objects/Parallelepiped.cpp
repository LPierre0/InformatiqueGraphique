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
    glm::vec3 p1 = center + glm::vec3(-width / 2, -height / 2, -depth / 2);
    glm::vec3 p2 = center + glm::vec3(width / 2, -height / 2, -depth / 2);
    glm::vec3 p3 = center + glm::vec3(width / 2, height / 2, -depth / 2);
    glm::vec3 p4 = center + glm::vec3(-width / 2, +height / 2, -depth / 2);
    glm::vec3 p5 = center + glm::vec3(-width / 2, -height / 2, depth / 2);
    glm::vec3 p6 = center + glm::vec3(width / 2, -height / 2, depth / 2);
    glm::vec3 p7 = center + glm::vec3(width / 2, height / 2, depth / 2);
    glm::vec3 p8 = center + glm::vec3(-width / 2, height / 2, depth / 2);

    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p5);
    points.push_back(p6);
    points.push_back(p7);
    points.push_back(p8);

    indices = {
        0, 1, 2, 0, 2, 3,  // Face avant 
        4, 5, 6, 4, 6, 7,  // Face arrière 
        0, 1, 5, 0, 5, 4,  // Face inférieure 
        3, 2, 6, 3, 6, 7,  // Face supérieure 
        0, 3, 7, 0, 7, 4,  // Face gauche
        1, 5, 6, 1, 6, 2   // Face droite 
    };
}


void Parallelepiped::calculate_normals_inv(){
    for (int i = 0; i < normals.size(); i++){
        normalsInv.push_back(normals[i] * -1.0f);
    }
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

        glm::vec3 normal = calculateNormal(p1, p2, p3);

        norm_to_treat[ind_p1] += normal;
        nb_norm[ind_p1] += 1.0f;

        norm_to_treat[ind_p2] += normal;
        nb_norm[ind_p2] += 1.0f;

        norm_to_treat[ind_p3] += normal;
        nb_norm[ind_p3] += 1.0f;
    }

    for (int i = 0; i < nb_points; i++){
        normals.push_back(norm_to_treat[i] / nb_norm[i]);
    }

    calculate_normals_inv();
}


void Parallelepiped::calculate_texture_coordinates(){
    glm::vec3 maxPoint = points[0];
    for (const auto& p : points) {
        maxPoint.x = std::max(maxPoint.x, p.x);
        maxPoint.y = std::max(maxPoint.y, p.y);
        maxPoint.z = std::max(maxPoint.z, p.z);
    }


    glm::vec3 minPoint = points[0];
    for (const auto& p : points) {
        minPoint.x = std::min(minPoint.x, p.x);
        minPoint.y = std::min(minPoint.y, p.y);
        minPoint.z = std::min(minPoint.z, p.z);
    }



    for (int i = 0; i < points.size(); i++){
        glm::vec3 normal = normals[i];
        glm::vec3 point = points[i];

        glm::vec2 uv;

        if (abs(normal.x) > abs(normal.y) && abs(normal.x) > abs(normal.z)) {
            uv = glm::vec2((point.z - minPoint.z) / (maxPoint.z - minPoint.z), 
                        (point.y - minPoint.y) / (maxPoint.y - minPoint.y)); // Projeté sur YZ
        } 
        else if (abs(normal.y) > abs(normal.x) && abs(normal.y) > abs(normal.z)) {
            uv = glm::vec2((point.x - minPoint.x) / (maxPoint.x - minPoint.x), 
                        (point.z - minPoint.z) / (maxPoint.z - minPoint.z)); // Projeté sur XZ
        } 
        else {
            uv = glm::vec2((point.x - minPoint.x) / (maxPoint.x - minPoint.x), 
                        (point.y - minPoint.y) / (maxPoint.y - minPoint.y)); // Projeté sur XY
        }

        texCoords.push_back(uv);
    }


}





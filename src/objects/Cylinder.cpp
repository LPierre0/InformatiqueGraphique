#include "Cylinder.h"


Cylinder::Cylinder(GLuint texture, glm::vec3 center, glm::vec3 color, glm::vec3 axis, float radius, float length, int subdiv1, int subdiv2)
    : Object(texture, center, color){
    this->radius = radius;
    this->length = length;
    this->subdiv1 = subdiv1;
    this->subdiv2 = subdiv2;
    this->axis = axis;
    this->base = center;


    printf("Base : %f, %f, %f", base.x, base.y, base.z);

    this->compute_points_and_normals();
    this->compute_indices();
    this->compute_texture_coordinates();
    this->compute_final();
    this->update();
    std::cout << "Cylindre crée avec succès" << std::endl;

}



void Cylinder::compute_points_and_normals(){
    glm::vec3 x;

    if (glm::abs(axis.y) < 0.99f) {  
        x = glm::vec3(0.0f, 1.0f, 0.0f);
    } else {
        x = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    glm::vec3 y = glm::cross(axis, x);

    this->points.clear();
    this->normals.clear();



    for (int i = 0; i < subdiv2; i++){
        float offset  = float( i ) / float(subdiv2 - 1);
        float offset2 = (offset - 0.5) * length;
        for (int j = 0; j < subdiv1; j++){
            float angle = 2.0f * glm::pi<float>() * float( j ) / float( subdiv1 );
            this->points.push_back(base + offset2 * axis + radius * glm::cos( angle ) * x + radius * glm::sin( angle ) * y);
            this->normals.push_back(glm::normalize((glm::cos( angle ) * x + glm::sin( angle ) * y)));
        }
    }

}


void Cylinder::compute_indices(){
    this->indices.clear();

    for (int i = 0; i < subdiv2 - 1; i++){
        for (int j = 0; j < subdiv1; j++){
            int current = i * subdiv1 + j;
            int next = i * subdiv1 + (j + 1) % subdiv1;
            int above = current + subdiv1;
            int aboveNext = next + subdiv1;

            // FACE 1
            this->indices.push_back(current);
            this->indices.push_back(next);
            this->indices.push_back(above);

            // FACE 2
            this->indices.push_back(next);
            this->indices.push_back(aboveNext);
            this->indices.push_back(above);
        }
    }

    int topCenterIndex = this->points.size(); 
    int bottomCenterIndex = topCenterIndex + 1;  

    this->points.push_back(base + axis * length * 0.5f);  
    this->points.push_back(base - axis * length * 0.5f);  

    this->normals.push_back(glm::normalize(axis));  
    this->normals.push_back(glm::normalize(-axis)); 

    for (int j = 0; j < subdiv1; j++){
        int current = (subdiv2 - 1) * subdiv1 + j;
        int next = (subdiv2 - 1) * subdiv1 + (j + 1) % subdiv1;

        this->indices.push_back(topCenterIndex);
        this->indices.push_back(current);
        this->indices.push_back(next);
    }

    for (int j = 0; j < subdiv1; j++){
        int current = j;
        int next = (j + 1) % subdiv1;

        this->indices.push_back(bottomCenterIndex);
        this->indices.push_back(next);
        this->indices.push_back(current);
    }
}

void Cylinder::compute_texture_coordinates(){
    for(int i = 0; i < this->points.size(); i++){
        this->texCoords.push_back(glm::vec2(0.0f, 0.0f));
    }
}

void Cylinder::update(){
    this->compute_points_and_normals();
    this->compute_indices();
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, final.size() * sizeof(float), final.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
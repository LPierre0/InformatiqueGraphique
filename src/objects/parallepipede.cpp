#include "parallepipede.h"


PARALLEPIPEDE::PARALLEPIPEDE(std::vector<glm::vec3> points, std::vector<std::vector<int>> faces, glm::vec3 center, GLuint texture){
    this->points = points;
    this->faces = faces;
    this->center = center;
    this->texture = texture;
}


void PARALLEPIPEDE::draw(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, final.size() * sizeof(glm::vec3), final.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}


void PARALLEPIPEDE::calculateIndices(){
    for (auto& face : this->faces){
        this->indices.push_back(face[0]);
        this->indices.push_back(face[1]);
        this->indices.push_back(face[2]);
        this->indices.push_back(face[3]);
        this->indices.push_back(face[4]);
        this->indices.push_back(face[5]);
    }
}


void PARALLEPIPEDE::getFinalData(){
    this->calculateNormals();


    for(unsigned int i = 0; i < points.size(); i++){
        this->final.push_back(points[i] + this->center);
        this->final.push_back(normals[i]);
    }

    for(unsigned int i = 0; i < indices.size(); i++){
        indices[i] = indices[i] * 2;
    }


}

glm::vec3 calculateNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    glm::vec3 v1 = p2 - p1;  
    glm::vec3 v2 = p3 - p1;  
    glm::vec3 normal = glm::cross(v1, v2);  

    return glm::normalize(normal);
}

void PARALLEPIPEDE::calculateNormals(){
    for(auto& face : this->faces){
        glm::vec3 p1 = this->points[face[0]];
        glm::vec3 p2 = this->points[face[1]];
        glm::vec3 p3 = this->points[face[2]];

        glm::vec3 normal = calculateNormal(p1, p2, p3);

        this->normals.push_back(normal);
        this->normals.push_back(normal);
        this->normals.push_back(normal);

        
        glm::vec3 p4 = this->points[face[3]];
        glm::vec3 p5 = this->points[face[4]];
        glm::vec3 p6 = this->points[face[5]];

        glm::vec3 normal_bis = calculateNormal(p4, p5, p6);

        this->normals.push_back(normal_bis);
        this->normals.push_back(normal_bis);
        this->normals.push_back(normal_bis);

    }
}




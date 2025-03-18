#include "Sphere.h"

const float PI = 3.14159265358979323846;


Sphere::Sphere(GLuint texture, glm::vec3 center, glm::vec3 color, float radius, int sector_count, int stack_count)
    : Object(texture, center, color){
    
    this->radius = radius;
    this->sector_count = sector_count;
    this->stack_count = stack_count; 


    this->compute_points_and_normals();
    this->compute_indices();
    this->compute_texture_coordinates();
    this->compute_final();
    this->update();
    std::cout << "Sphere crée avec succès" << std::endl;
}



void Sphere::compute_points_and_normals(){
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal                           

    float sectorStep = 2 * PI / sector_count;
    float stackStep = PI / stack_count;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stack_count; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sector_count+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sector_count; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);  
            glm::vec3 tempP = {x, y, z};         // r * cos(u) * sin(v)
            points.push_back(tempP + center);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            glm::vec3 tempN = {nx, ny, nz};
            normals.push_back(tempN);

        }
    }
}


void Sphere::compute_indices(){
    int k1, k2;
    for(int i = 0; i < stack_count; ++i)
    {
        k1 = i * (sector_count + 1);     // beginning of current stack
        k2 = k1 + sector_count + 1;      // beginning of next stack

        for(int j = 0; j < sector_count; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stack_count-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}


glm::vec2 compute_uv(glm::vec3 point, glm::vec3 center){
    glm::vec3 n = glm::normalize(point - center);  
    float u = 0.5f - atan2(n.z, n.x) / (2.0f * PI); 
    float v = 1.0f - (0.5f + n.y * 0.5f); 

    return glm::vec2(u, v);
}



void Sphere::compute_texture_coordinates(){
    texCoords.clear();
    for (const auto& p : points){
        glm::vec2 uv = compute_uv(p, center);
        texCoords.push_back(uv);
    }
}


void Sphere::update(){
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

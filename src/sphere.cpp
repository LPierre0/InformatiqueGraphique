#include "sphere.h"
#include "menu.h"
const float PI = 3.14159265358979323846;


Sphere::Sphere(glm::vec3 center, float radius, int sectorCount, int stackCount){
    set(center, radius, sectorCount, stackCount);
}

void Sphere::set(glm::vec3 center, float radius, int sectorCount, int stackCount){
    this->center = center;
    this->radius = radius;
    this->sectorCount = sectorCount;
    this->stackCount = stackCount;
}


void Sphere::calculatePointsAndNormals(){
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal                           

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
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


void Sphere::calculateIndices(){
    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
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
            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}


void Sphere::finalData(){

    for(unsigned int i = 0; i < points.size(); i++){
        final.push_back(points[i]);
        final.push_back(normals[i]);
    }


    for(unsigned int i = 0; i < indices.size(); i++){
        indices[i] = indices[i] * 2;
    }
}

void Sphere::update(float radius, int sectorCount, int stackCount){
    set(cen, radius, sectorCount, stackCount);
    indices.clear();
    normals.clear();
    points.clear();
    final.clear();
    calculatePointsAndNormals();
    calculateIndices();
    finalData();
}


void Sphere::draw(){
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
    if (updateSphere){
        update(radiusMenu, sectorCountMenu, stackCountMenu);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, final.size() * sizeof(glm::vec3), final.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
        updateSphere = false;
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}


void Sphere::drawIntersection(Ray rayon){
    Sphere sphereIntersection((this->t * rayon.direction + rayon.origin), 0.01f, 36, 18);
    sphereIntersection.calculatePointsAndNormals();
    sphereIntersection.calculateIndices();
    sphereIntersection.finalData();
    sphereIntersection.draw();
}

bool Sphere::intersect(Ray r){
    float t0, t1;
    glm::vec3 L = center - r.origin;
    float tca = glm::dot(L, r.direction);
    if (tca < 0 ) return false;
    float d2 = glm::dot(L, L) - tca * tca;
    if (d2 > radius * radius) return false;
    float thc = sqrt(radius * radius - d2);
    t0 = tca - thc;
    t1 = tca + thc;
    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // If t0 is negative, let's use t1 instead.
        if (t0 < 0) return false; // Both t0 and t1 are negative.
    }

    t = t0;

    return true;
}
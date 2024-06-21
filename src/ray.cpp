#include "ray.h"



/*
class Ray {
public:
    glm::vec3 origin; 
    glm::vec3 end;
    glm::vec3 direction; 

    double mouseX, mouseY;

    Ray(const glm::vec3& o, double mouseX, double mouseY);
    ~Ray() {};

    void calculateDirection(double screenWidht, double screenHeight);
    void calculateEnd();
    void draw();
};
*/

void Ray::calculateDirection(int screenWidth, int screenHeight, glm::mat4 projectionMatrix, glm::mat4 view){
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    float z = 1.0f; // Coordonnée Z dans l'espace de projection est 1
    glm::vec3 realCoord = glm::vec3(x, y, z);


    glm::vec4 viewCoords = glm::inverse(projectionMatrix) * glm::vec4(realCoord, 1.0f);
    viewCoords /= viewCoords.w;

    direction = glm::normalize(glm::vec3(glm::inverse(view) * viewCoords));
}


void Ray::calculateEnd(){
    float distance = 100.0f;
    end = origin + direction * distance;
}

void Ray::draw() {
    // Définition des points de la ligne
    std::vector<glm::vec3> t;
    t.push_back(origin);
    t.push_back(end);

    // Création et configuration du VAO et du VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, t.size() *  sizeof(glm::vec3), t.data(), GL_STATIC_DRAW);

    // Spécification des attributs de vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Dessin de la ligne
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
}

void Ray::update(int screenWidht, int screenHeight, glm::mat4 projectionMatrix, glm::mat4 view, const glm::vec3& o, double mX, double mY){
    this->origin = o; 
    this->mouseX = mX; 
    this->mouseY = mY;
    calculateDirection(screenWidht, screenHeight, projectionMatrix, view);
    calculateEnd();
    draw();
}


#include "Object.h"



Object::Object(GLuint texture, glm::vec3 center, glm::vec3 color)
{
    this->center = center;
    this->texture = texture;
    this->use_texture = (texture != 0);
    this->color = color;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    
    std::cout << "Objet construit proprement. \n" << std::endl;
}

Object::~Object() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    std::cout << "Objet détruit proprement. \n" << std::endl;
}



void Object::compute_final(){
    this->final.clear();
    for (int i = 0; i < points.size(); i++){
        glm::vec3 point = points[i];
        glm::vec3 normal = normals[i];
        glm::vec2 texCoord = texCoords[i];

        final.push_back(point[0]);
        final.push_back(point[1]);
        final.push_back(point[2]);

        final.push_back(normal[0]);
        final.push_back(normal[1]);
        final.push_back(normal[2]);

        final.push_back(texCoord[0]);
        final.push_back(texCoord[1]);


    }
}


void Object::save_final_data() {
    std::ofstream file("/home/pierre/Projects/InformatiqueGraphique/data_points.txt"); 
    std::cout << "saving final data" << std::endl;
    if (!file) {
        std::cerr << "Erreur d'ouverture du fichier" << std::endl;
        return;
    }

    for (size_t i = 0; i < final.size(); i++) {
        file << final[i];
        if ((i + 1) % 8 == 0) 
            file << '\n'; 
        else 
            file << ";";  
    }

    file.close();
}

void Object::update_mesh(){
    this->compute_points();
    this->compute_normals();
    this->compute_indices();
    this->compute_final();
}

void Object::update(){
    if (use_texture) glBindTexture(GL_TEXTURE_2D, texture);
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

void Object::set_texture(GLuint texture){
    this->texture = texture;
}

void Object::bind() const{
    glBindVertexArray(VAO);
}

void Object::set_center(glm::vec3 new_center){
    this->center = new_center;
}

void Object::self_print(){
    std::cout << "PRINT THE VALUES OF THE OBJECT" << std::endl;

    for (auto& p : points){
        std::cout << "x : " << p.x << "y :" << p.y << "z : " << p.z << std::endl;
    }

}

std::size_t Object::getIndexCount() const{
    return this->indices.size();
}


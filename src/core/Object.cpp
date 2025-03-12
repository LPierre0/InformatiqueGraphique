#include "Object.h"



Object::Object(GLuint texture, glm::vec3 center)
{
    this->center = center;
    this->texture = texture;

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



void Object::self_print(){
    for (auto& p : points){
        std::cout << "Point : " << "x : " << p.x << "y : " << p.y << "z : " << p.z << std::endl;
    }
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

void Object::update(){
    this->compute_points();
    this->compute_normals();
    this->compute_final();
    
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


    // for (size_t i = 0; i < final.size() / 8; i++) {
    //     glm::vec3 vertexPos(final[8 * i], final[8 * i + 1], final[8 * i + 2]); // Position du sommet
    //     glm::vec3 normal(final[8 * i + 3], final[8 * i + 4], final[8 * i + 5]); // Normale

    //     // Créer une ligne qui part du sommet et va dans la direction de la normale
    //     normals_lines.push_back(vertexPos.x);
    //     normals_lines.push_back(vertexPos.y);
    //     normals_lines.push_back(vertexPos.z);

    //     normals_lines.push_back(vertexPos.x + normal.x * 0.2f); // 0.2f = taille de la normale
    //     normals_lines.push_back(vertexPos.y + normal.y * 0.2f);
    //     normals_lines.push_back(vertexPos.z + normal.z * 0.2f);
    // }


    // glGenBuffers(1, &normalsVBO);
    // glGenVertexArrays(1, &normalsVAO);
    // glBindVertexArray(normalsVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    // glBufferData(GL_ARRAY_BUFFER, normals_lines.size() * sizeof(GLfloat), normals_lines.data(), GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    // glEnableVertexAttribArray(0);



    // // Nettoyage
    // glBindVertexArray(0);

}

void Object::set_texture(GLuint texture){
    this->texture = texture;
}

void Object::bind() const{
    //glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
}

void Object::set_center(glm::vec3 new_center){
    this->center = new_center;
    this->update();
}



std::size_t Object::getIndexCount() const{
    return this->indices.size();
}
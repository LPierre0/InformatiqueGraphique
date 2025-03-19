#include "Animation.h"



Animation::Animation(std::shared_ptr<Cylinder> existing_cylinder, Shader &existing_shader) 
: my_cylinder(existing_cylinder) , my_shader(existing_shader) {
    articulations = {
        glm::vec3(0.5f, 0.0f, 0.0f),
        glm::vec3(-1.5f, 0.0f, 0.0f)
    };
    create_bone(2, articulations);

    for (auto& p : my_cylinder->points) { 
        initial_points.push_back(p);
    }

    compute_weights();
}

void Animation::compute_weights(){
    weights.clear();
    for (auto& point: initial_points){
        std::vector<float> distance_i;
        float weight_sum = 0.0f;

        for (auto& poignee : articulations){
            float distance = glm::distance(point, poignee);
            float weight = 1.0f / (distance * distance + 1e-6f); 
            distance_i.push_back(weight);
            weight_sum += weight;
        }

        for (int i = 0; i < distance_i.size(); i++) {
            distance_i[i] /= weight_sum;  
        }

        weights.push_back(distance_i);
    }
}


void Animation::create_bone(int nb_bones, std::vector<glm::vec3> articulations) {
    bones.clear(); 
    bones_mesh.clear();

    bones.push_back(std::make_unique<Bone>(articulations[0]));
    bones.push_back(std::make_unique<Bone>(articulations[1], bones[0].get()));

    bones_mesh.push_back(std::make_unique<Sphere>(0, bones[0]->position, glm::vec3(0.0f, 1.0f, 0.0f), 0.3f, 64, 512));
    bones_mesh.push_back(std::make_unique<Sphere>(0, bones[1]->position, glm::vec3(0.0f, 1.0f, 0.0f), 0.3f, 64, 512));
}

void Animation::transform_bone(int articulation_index, glm::mat4 transform) {
    bones[articulation_index]->transform = transform;
    this->update_bone_mesh();
    this->update_cylinder();
}

void Animation::update_bone_mesh() {
    for (size_t i = 0; i < bones_mesh.size(); i++) {
        glm::vec4 transformed_position = bones[i]->getGlobalTransform() * glm::vec4(bones[i]->position, 1.0f);
        bones_mesh[i]->set_center(glm::vec3(transformed_position));
        bones_mesh[i]->update_mesh();
        bones_mesh[i]->update();
    }
}

void Animation::update_cylinder() {
    my_shader.setBool("use_gpu", use_gpu);
    if (use_gpu){
        std::vector<glm::mat4> boneTransforms;
        boneTransforms.push_back(bones[0]->getGlobalTransform());
        boneTransforms.push_back(bones[1]->getGlobalTransform());

        my_shader.setMat4("transform_1", boneTransforms[0]);
        my_shader.setMat4("transform_2", boneTransforms[1]);
    }
    else{
        int nb_points = my_cylinder->points.size();
        for (size_t i = 0; i < nb_points; i++) {
            glm::vec3 transformed_point(0.0f);
    
            for (size_t j = 0; j < articulations.size(); j++) {
                glm::mat4 global_transform = bones[j]->getGlobalTransform();
                
                glm::vec4 transformed = global_transform * glm::vec4(initial_points[i], 1.0f);
    
                float weight = weights[i][j];  
                transformed_point += weight * glm::vec3(transformed);
            }
    
            my_cylinder->points[i] = transformed_point;
        }
        final_cylinder();
        update_cylinder_vao();

    }
}

void Animation::set_initial_pos() {
    my_shader.setBool("use_gpu", false);
    for (size_t i = 0; i < my_cylinder->points.size(); ++i) {
        my_cylinder->points[i] = initial_points[i];  
    }
    final_cylinder(); 
    update_cylinder_vao();     

    for (size_t i = 0; i < bones_mesh.size(); ++i) {
        bones_mesh[i]->set_center(glm::vec3(articulations[i]));  
        bones_mesh[i]->update_mesh();  
        bones_mesh[i]->update();      
    }
}


void Animation::final_cylinder(){
    my_cylinder->final.clear();
    for (int i = 0; i < my_cylinder->points.size(); i++){
        glm::vec3 point = my_cylinder->points[i];
        glm::vec3 normal = my_cylinder->normals[i];
        glm::vec2 texCoord = my_cylinder->texCoords[i];
        std::vector<float> weight = weights[i];

        my_cylinder->final.push_back(point[0]);
        my_cylinder->final.push_back(point[1]);
        my_cylinder->final.push_back(point[2]);

        my_cylinder->final.push_back(normal[0]);
        my_cylinder->final.push_back(normal[1]);
        my_cylinder->final.push_back(normal[2]);

        my_cylinder->final.push_back(texCoord[0]);
        my_cylinder->final.push_back(texCoord[1]);

        my_cylinder->final.push_back(weight[0]);
        my_cylinder->final.push_back(weight[1]);

    } 
}


void Animation::update_cylinder_vao(){
    glBindTexture(GL_TEXTURE_2D, my_cylinder->texture);
    glBindVertexArray(my_cylinder->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, my_cylinder->VBO);
    glBufferData(GL_ARRAY_BUFFER, my_cylinder->final.size() * sizeof(float), my_cylinder->final.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_cylinder->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, my_cylinder->indices.size() * sizeof(int), my_cylinder->indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
}
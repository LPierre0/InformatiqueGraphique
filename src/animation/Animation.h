#ifndef Animation_H
#define Animation_H

#include "../include.h"
#include "../objects/Cylinder.h"
#include "../objects/Sphere.h"
#include "../core/Object.h"
#include "Bone.h"




class Animation
{
    public:
        Animation(std::shared_ptr<Cylinder> existing_cylinder, Shader &existing_shader);
        
        std::shared_ptr<Cylinder> my_cylinder;
        std::vector<glm::vec3> initial_points;
        std::vector<glm::vec3> articulations;
        std::vector<std::shared_ptr<Object>> bones_mesh;
        std::vector<std::vector<float>> weights;
        std::vector<std::shared_ptr<Bone>> bones;
        Shader my_shader;
        bool use_gpu = true;
        bool initial = false;



        void create_bone(int nb_bones, std::vector<glm::vec3> articulations);
        void transform_bone(int articulation_index, glm::mat4 transform);
        void update_bone_mesh();
        void compute_weights();
        void update_cylinder();
        void set_initial_pos();
        void final_cylinder();
        void update_cylinder_vao();

    private:

        
};

#endif

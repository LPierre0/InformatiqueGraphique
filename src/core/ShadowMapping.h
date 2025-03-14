#ifndef SHADOW_MAP
#define SHADOW_MAP

#include "../include.h"



class ShadowMap{
    public:
        ShadowMap(const char * path_depth_shader_vs, const char * path_depth_shader_fs, unsigned int width, unsigned int height, float near_plane, float far_plane, glm::mat4 model, float cutoff, glm::vec3 light_pos, glm::vec3 light_look_at);

        const char * path_detph_shader;
        unsigned int width; unsigned int height; 

        void update(float cutoff, glm::vec3 light_pos, glm::vec3 light_look_at);
        void set_shader();


        glm::mat4 biaisMatrix;
        glm::mat4 model;
        glm::mat4 depthMvp;
        glm::mat4 depthBiaisMvp;
        float near_plane; float far_plane; float cutoff;

        glm::vec3 light_pos; glm::vec3 light_look_at;

        unsigned int depthMap;
        
    private:
        Shader depth_shader;
        unsigned int depthMapFBO;
        

};

#endif
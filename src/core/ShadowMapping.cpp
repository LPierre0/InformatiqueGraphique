#include "ShadowMapping.h"


ShadowMap::ShadowMap(const char * path_depth_shader_vs, const char * path_depth_shader_fs, 
    unsigned int width, unsigned int height, float near_plane, float far_plane, glm::mat4 model, 
    float cutoff, glm::vec3 light_pos, glm::vec3 light_look_at) 
: depth_shader(path_depth_shader_vs, path_depth_shader_fs){

    this->width = width;
    this->height = height;

    this->far_plane = far_plane;
    this->near_plane = near_plane;
    this->model = model;

    glGenFramebuffers(1, &this->depthMapFBO);  
    glGenTextures(1, &this->depthMap);
    glBindTexture(GL_TEXTURE_2D, this->depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Vérification du framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Depth framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    this->biaisMatrix = glm::mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    this->update(cutoff, light_pos, light_look_at);
    
}



void ShadowMap::update(float cutoff, glm::vec3 light_pos, glm::vec3 light_look_at){

    this->cutoff = cutoff; this->light_pos = light_pos; this->light_look_at = light_look_at;


    glm::mat4 light_projection = glm::perspective(glm::radians(this->cutoff * 2.0f), 
                                              (float)this->width / (float)this->height, 
                                              0.1f, 50.0f);

    glm::mat4 light_view = glm::lookAt(this->light_pos, 
                                    this->light_look_at, 
                                    glm::vec3( 0.0f, 1.0f,  0.0f));  

    this->depthMvp = light_projection * light_view * this->model; 
    this->depthBiaisMvp = this->biaisMatrix * this->depthMvp;
}


void ShadowMap::set_shader(){
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    depth_shader.use();
    depth_shader.setMat4("depthMvp", depthMvp);
}

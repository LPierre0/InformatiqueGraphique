#ifndef OBJECT_H
#define OBJECT_H


#include "../include.h"

class Object
{
    public:
        Object(GLuint texture, glm::vec3 center);
        ~Object();

        void bind() const;
        void update();
        
        void bind_vao();
        unsigned int get_vao();
        unsigned int get_vbo();
        unsigned int get_ebo();

        void move_center(glm::vec3 new_center);
        std::size_t getIndexCount() const;
        glm::vec3 get_center();

        void set_center(glm::vec3 new_center);
        void compute_final();
        virtual void compute_points(){
                return;
        }
        virtual void compute_normals(){
                return;
        }

        void self_print();
        void save_final_data();

        GLuint get_texture();

        void set_texture(GLuint texture);

        unsigned int VAO, VBO, EBO;
        std::vector<glm::vec3> points;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        GLuint normalsVBO, normalsVAO;

        std::vector<GLfloat> normals_lines;
        
        std::vector<int> indices;

        std::vector<float> final;
/*
        FORME DE FINAL : 


        points   // normale  // normale inverse  //  coordonnées de texture

        0.0 0.0 0.0 // 0.0 0.0 0.0 // 0.0 0.0 0.0 // 0.0 0.0


*/



        glm::vec3 center;


        GLuint texture;

};


#endif // OBJECT_H
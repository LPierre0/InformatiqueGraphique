#include "TextureManager.h"


unsigned int TextureManager::load_texture(std::string texture_path){
    if (textures.find(texture_path) == textures.end()){
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width_text, height_text, nrChannels;

        std::string path = get_texture_path(texture_path);
        std::cout << path << std::endl;
        const char *cpath = path.c_str();
        unsigned char *data = stbi_load(cpath, &width_text, &height_text, &nrChannels, 0);
        if (data)
        {
            std::cout << "Width: " << width_text << ", Height: " << height_text << ", Channels: " << nrChannels << std::endl;

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_text, height_text, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            throw std::invalid_argument("Need a textures knowned the actual path is  : " + path);
        }
        stbi_image_free(data);
        textures[texture_path] = texture;
    }

    return textures[texture_path];
}


size_t TextureManager::get_size(){
    return textures.size();
}
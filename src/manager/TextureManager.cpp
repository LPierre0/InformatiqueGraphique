#include "TextureManager.h"


unsigned int TextureManager::load_texture(std::string texture_path){
    if (textures.find(texture_path) == textures.end()){
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        std::cout << "LOADING TEXTURE" << std::endl;
        // définit les options de la texture actuellement liée
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // charge et génère la texture
        int width_text, height_text, nrChannels;
        std::cout << "GET PATH" << std::endl;

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
            std::cout << "Failed to load texture" << std::endl;
            return -1;
        }
        stbi_image_free(data);
        textures[texture_path] = texture;
    }

    return textures[texture_path];
}



size_t TextureManager::get_size(){
    return textures.size();
}
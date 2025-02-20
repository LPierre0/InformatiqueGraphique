#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "../include.h"
#include <map>

class TextureManager{
    public:
        unsigned int load_texture(std::string texture_path);
        size_t get_size();

    private:
        std::map<std::string, unsigned int> textures;


};


#endif

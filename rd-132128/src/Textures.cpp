#include "Textures.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unordered_map<std::string, std::int32_t> Textures::idMap = std::unordered_map<std::string, std::int32_t>();
int lastId = -9999999;

std::int32_t Textures::loadTexture(const char* resourceName, std::int32_t mode) {
    try {
        if (idMap.find(resourceName) != idMap.end()) {
            return idMap.at(resourceName);
        }

        std::uint32_t ib[1];
        glGenTextures(1, ib);
        int id = ib[0];
        idMap.insert(std::pair<std::string, std::int32_t>(resourceName, id));
        std::cout << resourceName << " -> " << id << std::endl;

        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

        int w, h, nrChannels;
        std::uint8_t* img = stbi_load(resourceName, &w, &h, &nrChannels, 0);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);
        stbi_image_free(img);
        
        return id;
    } catch (std::exception e) {
        std::cout << "!!" << std::endl;
    }
}
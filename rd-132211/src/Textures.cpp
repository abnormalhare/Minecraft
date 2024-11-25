#include "Textures.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unordered_map<std::string, std::int32_t>* Textures::idMap;
std::int32_t Textures::lastId = -9999999;

std::int32_t Textures::loadTexture(const char* resourceName, std::int32_t mode) {
    try {
        if (idMap == nullptr) {
            idMap = new std::unordered_map<std::string, std::int32_t>();
        }
        if (!idMap->empty()) {
            std::string rNameStr(resourceName);

            if (idMap->find(rNameStr) != idMap->end()) {
                return idMap->at(rNameStr);
            }
        }

        std::uint32_t ib[1];
        glGenTextures(1, ib);
        int id = ib[0];
        bind(id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

        // read in image
        int w, h, nrChannels;
        std::uint8_t* img = stbi_load(resourceName, &w, &h, &nrChannels, 4);
        
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);

        stbi_image_free(img);
        return id;
    } catch (const std::exception& e) {
        std::cerr << "!!" << std::endl;
        exit(1);
    }
}

void Textures::bind(std::int32_t id) {
    if (id != lastId) {
        glBindTexture(GL_TEXTURE_2D, id);
        lastId = id;
    }
}
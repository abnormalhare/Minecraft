#include "Textures.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unordered_map<std::string, std::int32_t> Textures::idMap;
std::int32_t Textures::lastId = -9999999;

std::int32_t Textures::loadTexture(const std::string resourceName, std::int32_t mode) {
    try {
        if (!idMap.empty()) {
            if (idMap.find(resourceName) != idMap.end()) {
                return idMap.at(resourceName);
            }
        }

        std::uint32_t ib[1];
        glGenTextures(1, ib);
        std::int32_t id = ib[0];
        idMap[resourceName] = id;

        std::cout << resourceName << " -> " << id << std::endl;

        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

        // read in image
        int w, h, nrChannels;
        std::uint8_t* img = stbi_load(resourceName.c_str(), &w, &h, &nrChannels, 4);
        
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);

        stbi_image_free(img);
        return id;
    } catch (const std::exception& e) {
        std::cerr << "!!" << std::endl;
        exit(1);
    }
}
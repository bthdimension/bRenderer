#ifndef B_MATERIAL_DATA_H
#define B_MATERIAL_DATA_H

#include <unordered_map>

struct MaterialData
{
    typedef std::unordered_map<std::string, std::string>    Textures;
    typedef std::unordered_map<std::string, vmml::vec3f>    Vectors;
    typedef std::unordered_map<std::string, float>          Scalars;
    
    Textures    textures;
    Vectors     vectors;
    Scalars     scalars;
    
    std::string name;
};

#endif /* defined(B_MATERIAL_DATA_H) */

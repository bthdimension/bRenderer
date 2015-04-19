//
//  MaterialData.h
//  Framework
//
//  Created by David Steiner on 4/28/14.
//
//

#ifndef FRAMEWORK_MATERIAL_DATA_H
#define FRAMEWORK_MATERIAL_DATA_H

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

#endif

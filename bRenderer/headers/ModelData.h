//
//  TextureData.h
//  Framework
//
//  Created by David Steiner.
//
//

#ifndef B_MODEL_DATA_H
#define B_MODEL_DATA_H

#include <memory>
#include <string>
#include <map>
#include "Renderer_GL.h"
#include "GeometryData.h"

class ModelDataImpl;

class ModelData
{
    friend ModelDataImpl;
    
public:
    typedef std::map< std::string, GeometryDataPtr >    GroupMap;
    
    ModelData(const std::string &fileName, bool flipT = false, bool flipZ = false);
    ModelData(bool flipT = false, bool flipZ = false);
    ~ModelData();
    
    ModelData &load(const std::string &fileName);
    
    GroupMap getData() const;
    
private:
    ModelDataImpl   *_impl;
    bool            _flipT;
    bool            _flipZ;
};

#endif /* defined(B_MODEL_DATA_H) */

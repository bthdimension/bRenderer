//
//  Model.h
//  Framework
/*
    Loads a model and defines different functions for loading different model types(PLY, OBJ)
    Creates geometry for each group depending on model type.
*/
//  Created by Rahul Mukhi on 1/16/13.
//  Reworked by David Steiner
//

#ifndef B_MODEL_H
#define B_MODEL_H

#include <memory>
#include <string>
#include <map>
#include "IDrawable.h"
#include "Geometry.h"
#include "Material.h"
#include "Renderer_GL.h"

class Renderer;
class ModelData;

class Model: public IDrawable
{
public:
    
	explicit Model(Renderer *r, const ModelData &modelData);
    ~Model();
    
    void draw(GLenum mode = GL_TRIANGLES);
    void draw(const std::string &groupName, GLenum mode = GL_TRIANGLES);
    
    typedef std::map< std::string, Geometry > GroupMap;
    
    MaterialPtr getMaterial()   { return _material; }
    
    void setMaterial(MaterialPtr arg)
    {
        _material = arg;
        
        for (auto i = _groups.begin(); i != _groups.end(); ++i)
        {
            i->second.setMaterial(_material);
        }
    }
    
    GroupMap &getGroups()   { return _groups; }

private:
    GroupMap        _groups;
    MaterialPtr     _material;
};

typedef std::shared_ptr<Model> ModelPtr;

#endif /* defined(B_MODEL_H) */

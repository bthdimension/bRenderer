#version 120

uniform lowp mat4 ViewMatrix;
uniform lowp mat4 ModelMatrix;
uniform lowp mat4 NormalMatrix;
uniform lowp mat4 ModelViewMatrix;
uniform lowp mat4 ProjectionMatrix;

uniform lowp vec4 LightPos;

uniform lowp vec3 Ka;
uniform lowp vec3 Kd;
uniform lowp vec3 Ks;

uniform lowp vec3 Ia;
uniform lowp vec3 Id;
uniform lowp vec3 Is;

uniform mediump float Specularity;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying lowp vec4 posVarying;       // pos in world space
varying lowp vec3 normalVarying;    // normal in world space

// TODO: calculate TBN matrix

void main()
{
    posVarying = ModelMatrix * Position;
    normalVarying = normalize(mat3(NormalMatrix) * Normal);
    
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;
}

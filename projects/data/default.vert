$B_SHADER_VERSION
#ifdef GL_ES
precision highp float;
#endif

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

//Light
uniform float numLights;
//L0
uniform vec4 lightPosition_0;
//L1
uniform vec4 lightPosition_1;
//L2
uniform vec4 lightPosition_2;
//L3
uniform vec4 lightPosition_3;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying vec4 texCoordVarying;

varying vec4 posWorldSpace;       // pos in View Space

varying vec3 tangentSurface2light_0;
varying vec3 tangentSurface2light_1;
varying vec3 tangentSurface2light_2;
varying vec3 tangentSurface2light_3;

void main()
{
    posWorldSpace = ModelMatrix*Position;
    
    //TBN
    vec3 vertexNormal_WorldSpace = mat3(ModelMatrix) * normalize(Normal);
    vec3 vertexTangent_WorldSpace = mat3(ModelMatrix) * normalize(Tangent);
    vec3 vertexBitangent_WorldSpace = mat3(ModelMatrix) * normalize(Bitangent);
    
    //transpose matrix
    vec3 firstRow = vec3(vertexTangent_WorldSpace.x, vertexBitangent_WorldSpace.x, vertexNormal_WorldSpace.x);
    vec3 secondRow = vec3(vertexTangent_WorldSpace.y, vertexBitangent_WorldSpace.y, vertexNormal_WorldSpace.y);
    vec3 thirdRow = vec3(vertexTangent_WorldSpace.z, vertexBitangent_WorldSpace.z, vertexNormal_WorldSpace.z);
    mat3 TBN = mat3(firstRow,secondRow,thirdRow);
    
    // light 0
    if(numLights >= 1.0)
    {
        tangentSurface2light_0 = normalize(TBN*(lightPosition_0.xyz - posWorldSpace.xyz));
    }
    // light 1
    if(numLights >= 2.0)
    {
        tangentSurface2light_1 = normalize(TBN*(lightPosition_1.xyz - posWorldSpace.xyz));
    }
    // light 2
    if(numLights >= 3.0)
    {
        tangentSurface2light_2 = normalize(TBN*(lightPosition_2.xyz - posWorldSpace.xyz));
    }
	// light 3
    if(numLights >= 4.0)
    {
        tangentSurface2light_3 = normalize(TBN*(lightPosition_3.xyz - posWorldSpace.xyz));
    }
    
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;
}

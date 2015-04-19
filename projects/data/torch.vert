# version 120

uniform lowp mat4 ViewMatrix;
uniform lowp mat4 ModelMatrix;
uniform lowp mat4 NormalMatrix;
uniform lowp mat4 ModelViewMatrix;
uniform lowp mat4 ProjectionMatrix;
//uniform lowp mat4 ViewMatrixInverse;

//Light
uniform lowp vec4 LightPos;
uniform lowp float lightIntensity;
uniform lowp float flickeringLight;
varying lowp vec4 flickeringLightPos;
varying lowp float flickeringLightIntensity;

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
varying lowp vec4 posViewSpace;       // pos in View Space
varying lowp vec3 normalVarying;    // normal in View Space

//cplusplusguy
//varying lowp vec3 tangentSurface2light;
//varying lowp vec3 tangentSurface2view;

//varying lowp mat3 TBN;

void main()
{
    
    //flickering based on position
    flickeringLightPos = LightPos+12.0;
    flickeringLightPos.x += 3.0*sin(flickeringLightPos.y * 4.0 * 1.0 * 3.14159 + flickeringLight);
    flickeringLightPos.y += 1.7*sin(flickeringLightPos.x * 4.0 * 2.0 * 3.14159 + flickeringLight);
    //based on intensity
    flickeringLightIntensity = lightIntensity;
//    flickeringLightIntensity = 20.0 + 0.65*sin(flickeringLight);
    
    
	lowp vec3 HackNormal = cross(Tangent,Bitangent);
    posVarying = ModelMatrix * Position;
    posViewSpace = ViewMatrix*posVarying;
    normalVarying = normalize(mat3(ViewMatrix) * mat3(NormalMatrix) * HackNormal);
    
    
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;
}

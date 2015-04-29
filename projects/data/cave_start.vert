$B_SHADER_VERSION

uniform lowp mat4 ViewMatrix;
uniform lowp mat4 ModelMatrix;
uniform lowp mat3 NormalMatrix;
uniform lowp mat4 ModelViewMatrix;
uniform lowp mat4 ProjectionMatrix;
uniform lowp mat3 NormalMatrixViewSpace;

//Light
uniform lowp vec4 LightPos;
uniform lowp float flickeringLight;
varying lowp vec4 flickeringLightPos;

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

varying lowp vec4 posWorldSpace;       // pos in world space
varying lowp vec4 posViewSpace;       // pos in View Space
varying lowp vec3 normalVarying;    // normal in view space

//cplusplusguy
varying lowp vec3 tangentSurface2light;
varying lowp vec3 tangentSurface2view;

varying lowp mat3 TBN;

void main()
{
    
    //flickering based on position
    flickeringLightPos = LightPos;
    flickeringLightPos.x += 1.4*sin(flickeringLightPos.y * 4.0 * 1.0 * 3.14159 + flickeringLight);
    flickeringLightPos.y += 1.7*sin(flickeringLightPos.x * 4.0 * 2.0 * 3.14159 + flickeringLight);
    
    posWorldSpace = ModelMatrix * Position;
    posViewSpace = ViewMatrix*ModelMatrix*Position;
//    normalVarying = normalize(NormalMatrixViewSpace * Normal);
    
    //TBN: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
    vec3 vertexNormal_cameraspace = mat3(ViewMatrix * ModelMatrix) * normalize(Normal);
    vec3 vertexTangent_cameraspace = mat3(ViewMatrix * ModelMatrix) * normalize(Tangent);
    vec3 vertexBitangent_cameraspace = mat3(ViewMatrix * ModelMatrix) * normalize(Bitangent);
    
    //transpose matrix
    vec3 firstRow = vec3(vertexTangent_cameraspace.x, vertexBitangent_cameraspace.x, vertexNormal_cameraspace.x);
    vec3 secondRow = vec3(vertexTangent_cameraspace.y, vertexBitangent_cameraspace.y, vertexNormal_cameraspace.y);
    vec3 thirdRow = vec3(vertexTangent_cameraspace.z, vertexBitangent_cameraspace.z, vertexNormal_cameraspace.z);
    TBN = mat3(firstRow,secondRow,thirdRow);
    
    //cplusplusguy
    tangentSurface2light = TBN*normalize(flickeringLightPos.xyz - posViewSpace.xyz);
    tangentSurface2view = TBN*normalize(-posViewSpace.xyz);
    
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;
}

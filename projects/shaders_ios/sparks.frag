
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

uniform mediump float Ns;

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;

uniform lowp float offset;

varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying lowp vec4 posVarying;       // pos in world space
varying lowp vec3 normalVarying;    // normal in world space

varying lowp vec4 testClr;

void main()
{    
    gl_FragColor = texture2D(DiffuseMap, texCoordVarying.st);
}

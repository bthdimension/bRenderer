
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
uniform lowp float transparency;

varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying lowp vec4 posVarying;       // pos in world space
varying lowp vec3 normalVarying;    // normal in world space

varying lowp vec4 testClr;

void main()
{
    //wave effect for flame
    lowp vec2 texcoord = texCoordVarying.st;
    texcoord.x += sin(texcoord.y * 4.0 * 1.0 * 3.14159 + offset) / 250.0;
    texcoord.y += sin(texcoord.x * 4.0 * 2.0 * 3.14159 + offset) / 100.0;
    
    lowp vec4 transparencyVec = vec4(1.0,1.0,1.0,transparency);
    lowp vec4 lightColor = vec4(1.0, 0.6, 1.0, 1.0);
    
    gl_FragColor = lightColor * texture2D(DiffuseMap, texcoord)*transparencyVec;
}

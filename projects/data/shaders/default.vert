
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec4 TexCoord;

varying vec4 colorVarying;

void main()
{
    vec4 pos = ModelViewMatrix * Position;  // vertex position in eye coordinates
    vec3 normal = normalize(NormalMatrix * Normal.xyz);

    colorVarying = vec4(vec3(0.5) + normal * 0.5, 1.0);
    
    gl_Position = ProjectionMatrix * pos;
}

$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform mat4 ProjectionMatrix;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying vec4 texCoordVarying;

varying vec4 posVarying;       // pos in world space
varying vec3 normalVarying;    // normal in world space

void main()
{
    posVarying = ModelMatrix * Position;
    normalVarying = normalize(mat3(NormalMatrix) * Normal);
    
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;
}

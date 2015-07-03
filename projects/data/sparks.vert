$B_SHADER_VERSION

uniform lowp mat4 ViewMatrix;
uniform lowp mat4 ModelMatrix;
uniform lowp mat4 NormalMatrix;
uniform lowp mat4 ProjectionMatrix;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying lowp vec4 texCoordVarying;

void main()
{
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;
}

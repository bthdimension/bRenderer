$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D DiffuseMap;

uniform vec3 ambientColor;

varying vec4 texCoordVarying;

void main()
{    
    gl_FragColor = vec4(ambientColor, 0.0) +  texture2D(DiffuseMap, texCoordVarying.st);
}

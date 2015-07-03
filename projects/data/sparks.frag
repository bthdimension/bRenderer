$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D DiffuseMap;
varying vec4 texCoordVarying;

void main()
{    
    gl_FragColor = texture2D(DiffuseMap, texCoordVarying.st);
}

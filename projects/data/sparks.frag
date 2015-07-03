$B_SHADER_VERSION

uniform sampler2D DiffuseMap;
varying lowp vec4 texCoordVarying;

void main()
{    
    gl_FragColor = texture2D(DiffuseMap, texCoordVarying.st);
}

$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform vec3 ambientColor;

uniform sampler2D DiffuseMap;

uniform
#ifdef GL_ES
highp   // offset gets very big, needs to be highp on iOS
#endif
float offset;

varying vec4 texCoordVarying;

void main()
{
    //wave effect for flame
    vec2 texcoord = texCoordVarying.st;
    texcoord.x += sin(texcoord.y * 12.0 + offset) / 250.0;
	texcoord.y += sin(texcoord.x * 24.0 + offset) / 100.0;
    
    vec4 lightColor = vec4(1.0, 0.7, 1.0, 1.0);
    
    gl_FragColor = vec4(ambientColor, 0.0) + lightColor * texture2D(DiffuseMap, texcoord);
}

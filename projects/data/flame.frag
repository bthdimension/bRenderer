$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D DiffuseMap;

uniform float offset;
uniform float transparency;

varying vec4 texCoordVarying;

void main()
{
    //wave effect for flame
    vec2 texcoord = texCoordVarying.st;
    texcoord.x += sin(texcoord.y * 4.0 * 1.0 * 3.14159 + offset) / 250.0;
    texcoord.y += sin(texcoord.x * 4.0 * 2.0 * 3.14159 + offset) / 100.0;
    
    vec4 transparencyVec = vec4(1.0,1.0,1.0,transparency);
    vec4 lightColor = vec4(1.0, 0.6, 1.0, 1.0);
    
    gl_FragColor = lightColor * texture2D(DiffuseMap, texcoord)*transparencyVec;
}

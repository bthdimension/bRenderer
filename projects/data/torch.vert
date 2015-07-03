$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

//Light
uniform float  numLights;
uniform vec4 lightPositionViewSpace_0;
uniform float lightIntensity_0;
uniform float flickeringLight;
varying vec4 flickeringLightPos;
varying float flickeringLightIntensity;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying vec4 texCoordVarying;

varying vec4 posViewSpace;       // pos in View Space
varying vec3 normalVarying;    // normal in View Space

void main()
{
    if(numLights > 0.0){
		//flickering based on position
		flickeringLightPos = lightPositionViewSpace_0+12.0;
		flickeringLightPos.x += 3.0*sin(flickeringLightPos.y * 4.0 * 1.0 * 3.14159 + flickeringLight);
		flickeringLightPos.y += 1.7*sin(flickeringLightPos.x * 4.0 * 2.0 * 3.14159 + flickeringLight);
		//based on intensity
		flickeringLightIntensity = lightIntensity_0;    
    
		posViewSpace = ModelViewMatrix * Position;
		normalVarying = normalize(mat3(ModelViewMatrix) * Normal);
        
		texCoordVarying = TexCoord;
	}
    gl_Position = ProjectionMatrix * posViewSpace;
}

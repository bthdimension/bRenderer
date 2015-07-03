$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

//Light
uniform float  numLights;
uniform vec4 lightPositionViewSpace_0;
uniform vec3 lightColor_0;
uniform float lightIntensity_0;
uniform float lightAttenuation_0;
uniform float flickeringLight;
varying vec4 flickeringLightPos;
varying float flickeringLightIntensity;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 ambientColor;

uniform sampler2D DiffuseMap;

varying vec4 texCoordVarying;

varying vec4 posViewSpace;       // pos in View Space
varying vec3 normalVarying;    // normal in View Space

void main()
{
	if(numLights > 0.0){
		//normal based on normal map
		vec3 n = normalize(normalVarying);
    
		//light vector (light is at camera position, that's why everything is in viewspace)
		vec3 l = normalize(flickeringLightPos.xyz - posViewSpace.xyz);
        
		//light intensity based on distance from light to object
		float distance = distance(posViewSpace, flickeringLightPos);
		float intensityBasedOnDist = flickeringLightIntensity/(lightAttenuation_0*distance*distance);
        
		intensityBasedOnDist = clamp(intensityBasedOnDist, 0.0, 1.0);
    
		//change light color based on distance (far = blue, near = yellow)
		vec3 LightColorBasedOnDist = lightColor_0;
		LightColorBasedOnDist.x += intensityBasedOnDist*2.2;    //red
		LightColorBasedOnDist.z -= intensityBasedOnDist*1.5;    //blue
		LightColorBasedOnDist = clamp(LightColorBasedOnDist, 0.0, 1.0);
    
		//light intensity based on angle between normal and light vector
		float intensity = max(dot(l, n), 0.0) ;
		intensity = clamp(intensity, 0.0, 1.0);

		//ambient light
		vec3 ambient  = Ka * ambientColor;
    
		//diffuse light
		vec3 diffuse  = LightColorBasedOnDist * Kd * intensityBasedOnDist * texture2D(DiffuseMap, texCoordVarying.st).xyz;
		diffuse = clamp(diffuse, 0.0, 1.0);
    
		gl_FragColor = vec4((ambient + diffuse), 1.0);
	}
	else{
		gl_FragColor = vec4(0.0,0.0,0.0,1.0);
	}
}

$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

//Light
uniform float  numLights;
//L0
uniform vec4   lightPosition_0;
uniform vec3   lightColor_0;
uniform float  lightIntensity_0;
uniform float  lightAttenuation_0;
//L1
uniform vec4   lightPosition_1;
uniform vec3   lightColor_1;
uniform float  lightIntensity_1;
uniform float  lightAttenuation_1;
//L2
uniform vec4   lightPosition_2;
uniform vec3   lightColor_2;
uniform float  lightIntensity_2;
uniform float  lightAttenuation_2;
//L3
uniform vec4   lightPosition_3;
uniform vec3   lightColor_3;
uniform float  lightIntensity_3;
uniform float  lightAttenuation_3;

uniform vec3 	ambientColor;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform sampler2D NormalMap;
uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;

varying vec4 texCoordVarying;

varying vec4 posWorldSpace;       // pos in View Space

varying vec3 tangentSurface2light_0;
varying vec3 tangentSurface2light_1;
varying vec3 tangentSurface2light_2;
varying vec3 tangentSurface2light_3;

void main()
{
    //normal based on normal map
    vec3 normalNormalMap = normalize(texture2D(NormalMap, texCoordVarying.st).xyz *2.0 - 1.0);

	//ambient light
    vec3 ambient  = clamp(ambientColor + Ka, 0.0, 1.0);

	//diffuse light
	vec3 diffuse = vec3(0.0,0.0,0.0);
    float distanceToLight = 0.0;
    float intensityBasedOnDist = 0.0;
    float intensity = 0.0;
    
    // light 0
    if(numLights >= 1.0)
    {
        //light intensity based on angle between normal and light vector
        intensity = max(dot(normalNormalMap,tangentSurface2light_0), 0.0) ;
        
        if(intensity > 0.0)
        {
            intensity = clamp(intensity, 0.0, 1.0);
            
            //light intensity based on distance from light to object
            distanceToLight = distance(posWorldSpace, lightPosition_0);
            intensityBasedOnDist = lightIntensity_0/(lightAttenuation_0*distanceToLight*distanceToLight);
            intensityBasedOnDist = clamp(intensityBasedOnDist, 0.0, 1.0);
            
            //diffuse light
            diffuse  += lightColor_0 * intensity * intensityBasedOnDist;
        }
    }
    // light 1
    if(numLights >= 2.0)
	{
        //light intensity based on angle between normal and light vector
        intensity = max(dot(normalNormalMap,tangentSurface2light_1), 0.0) ;
        
        if(intensity > 0.0)
        {
            intensity = clamp(intensity, 0.0, 1.0);
            
            //light intensity based on distance from light to object
            distanceToLight = distance(posWorldSpace, lightPosition_1);
            intensityBasedOnDist = lightIntensity_1/(lightAttenuation_1*distanceToLight*distanceToLight);
            intensityBasedOnDist = clamp(intensityBasedOnDist, 0.0, 1.0);
        
            //diffuse light
            diffuse  += lightColor_1 * intensity * intensityBasedOnDist;
        }
    }
    // light 2
    if(numLights >= 3.0)
    {
        //light intensity based on angle between normal and light vector
        intensity = max(dot(normalNormalMap,tangentSurface2light_2), 0.0) ;
        
        if(intensity > 0.0)
        {
            intensity = clamp(intensity, 0.0, 1.0);
            
            //light intensity based on distance from light to object
            distanceToLight = distance(posWorldSpace, lightPosition_2);
            intensityBasedOnDist = lightIntensity_2/(lightAttenuation_2*distanceToLight*distanceToLight);
            intensityBasedOnDist = clamp(intensityBasedOnDist, 0.0, 1.0);
            
            //diffuse light
            diffuse  += lightColor_2 * intensity * intensityBasedOnDist;
        }
    }
	// light 3
    if(numLights >= 4.0)
    {
        //light intensity based on angle between normal and light vector
        intensity = max(dot(normalNormalMap,tangentSurface2light_3), 0.0) ;
        
        if(intensity > 0.0)
        {
            intensity = clamp(intensity, 0.0, 1.0);
            
            //light intensity based on distance from light to object
            distanceToLight = distance(posWorldSpace, lightPosition_3);
            intensityBasedOnDist = lightIntensity_3/(lightAttenuation_3*distanceToLight*distanceToLight);
            intensityBasedOnDist = clamp(intensityBasedOnDist, 0.0, 1.0);
            
            //diffuse light
            diffuse  += lightColor_3 * intensity * intensityBasedOnDist;
        }
    }
    
	diffuse = diffuse * Kd * texture2D(DiffuseMap, texCoordVarying.st).xyz;

    gl_FragColor = vec4(clamp(ambient + diffuse, 0.0, 1.0), 1.0);
}

//#version 120

uniform lowp mat4 ViewMatrix;
uniform lowp mat4 ModelMatrix;
uniform lowp mat4 NormalMatrix;
uniform lowp mat4 ModelViewMatrix;
uniform lowp mat4 ProjectionMatrix;

//Light
uniform lowp vec4 LightPos;
uniform lowp vec4 LightColor;
uniform lowp float lightIntensity;
uniform lowp float attenuation;
uniform lowp float flickeringLight;
varying lowp vec4 flickeringLightPos;
varying lowp float flickeringLightIntensity;

uniform lowp vec3 Ka;
uniform lowp vec3 Kd;
//uniform lowp vec3 Ks;

uniform lowp vec3 Ia;
uniform lowp vec3 Id;
//uniform lowp vec3 Is;

uniform mediump float Ns;

uniform sampler2D NormalMap;
uniform sampler2D DiffuseMap;
//uniform sampler2D SpecularMap;

varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying lowp vec4 posVarying;       // pos in world space
varying lowp vec4 posViewSpace;       // pos in View Space
varying lowp vec3 normalVarying;    // normal in View Space

varying lowp vec4 testClr;

void main()
{
    //normal based on normal map
    lowp vec3 n = normalize(normalVarying);
    
    //light vector (light is at camera position, that's why everything is in viewspace)
    lowp vec3 l = normalize(flickeringLightPos.xyz - posViewSpace.xyz);
        
    //light intensity based on distance from light to object
    lowp float distance = distance(posViewSpace, flickeringLightPos);
    lowp float intensityBasedOnDist = flickeringLightIntensity/(attenuation*distance*distance);
        
    intensityBasedOnDist = clamp(intensityBasedOnDist, 0.0, 1.0);
    
    //change light color based on distance (far = blue, near = yellow)
    lowp vec4 LightColorBasedOnDist = LightColor;
    LightColorBasedOnDist.x += intensityBasedOnDist*2.2;    //red
//    LightColorBasedOnDist.y += intensityBasedOnDist*0.1;    //green
    LightColorBasedOnDist.z -= intensityBasedOnDist*1.5;    //blue
    LightColorBasedOnDist = clamp(LightColorBasedOnDist, 0.0, 1.0);
    
    //light intensity based on angle between normal and light vector
    lowp float intensity = max(dot(l, n), 0.0) ;
    intensity = clamp(intensity, 0.0, 1.0);

    //ambient light
    lowp vec3 ambient  = Ka * Ia;
    
    //diffuse light
    lowp vec3 diffuse  = Kd * intensityBasedOnDist *  Id * texture2D(DiffuseMap, texCoordVarying.st).xyz;
    diffuse = clamp(diffuse, 0.0, 1.0);
    
    // specular light based on specular map -> works without post processing
    mediump vec3 specular = vec3(0.0);
//    if (intensity > 0.0)
//    {
//        //l in view space
//        l = normalize(LightPos.xyz - posViewSpace.xyz);
//        mediump vec3 eye = normalize(tangentSurface2view);
//        mediump vec3 h = normalize(l + eye);
//        
//        mediump float specularityFromMap = texture2D(SpecularMap, texCoordVarying.st).x * 255.0;
//
//        specular = Ks * pow(max(dot(h,n), 0.0), specularityFromMap) * Is;
//    }
    
    gl_FragColor = LightColorBasedOnDist * vec4((ambient + diffuse + specular), 1.0);
}

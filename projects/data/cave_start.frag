$B_SHADER_VERSION

uniform lowp mat4 ViewMatrix;
uniform lowp mat4 ModelMatrix;
uniform lowp mat3 NormalMatrix;
uniform lowp mat4 ModelViewMatrix;
uniform lowp mat4 ProjectionMatrix;

//Light
uniform lowp vec4 LightPos;
uniform lowp vec4 LightColor;
uniform lowp float lightIntensity;
uniform lowp float attenuation;
uniform lowp float flickeringLight;
varying lowp vec4 flickeringLightPos;

uniform lowp vec3 Ka;
uniform lowp vec3 Kd;
uniform lowp vec3 Ks;

uniform lowp vec3 Ia;
uniform lowp vec3 Id;
uniform lowp vec3 Is;

uniform mediump float Ns;

uniform sampler2D NormalMap;
uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;

varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying lowp vec4 posWorldSpace;       // pos in world space
varying lowp vec4 posViewSpace;       // pos in View Space
varying lowp vec3 normalVarying;    // normal in view space

varying lowp vec4 testClr;

//cplusplusguy
varying lowp vec3 tangentSurface2light;
varying lowp vec3 tangentSurface2view;

varying lowp mat3 TBN;

void main()
{
    //normal based on normal map
    lowp vec3 n = normalize(texture2D(NormalMap, texCoordVarying.st).xyz *2.0 - 1.0);
    
    //light vector (light is at camera position, that's why everything is in viewspace)
    lowp vec3 l = normalize(tangentSurface2light);
        
    //light intensity based on distance from light to object
    lowp float distance = distance(posViewSpace, flickeringLightPos);
    lowp float intensityBasedOnDist = lightIntensity/(attenuation*distance*distance);
    intensityBasedOnDist = clamp(intensityBasedOnDist, 0.0, 1.0);
    
    //change light color based on distance (far = blue, near = orange)
    lowp vec4 LightColorBasedOnDist = LightColor;
    LightColorBasedOnDist.x += intensityBasedOnDist*0.22;    //red
    LightColorBasedOnDist.y -= intensityBasedOnDist*0.2;    //green
    LightColorBasedOnDist.z -= intensityBasedOnDist*0.8;    //blue
    LightColorBasedOnDist = clamp(LightColorBasedOnDist, 0.0, 1.0);
    
    //light intensity based on angle between normal and light vector
    lowp float intensity = max(dot(n,l), 0.0) ;
    intensity = clamp(intensity, 0.0, 1.0);

    //ambient light
    mediump vec3 ambient  = Ka * Ia;
    
    //diffuse light
    mediump vec3 diffuse  = Kd * intensity * intensityBasedOnDist *  Id * texture2D(DiffuseMap, texCoordVarying.st).xyz;
    diffuse = clamp(diffuse, 0.0, 1.0);
    
    
    

    gl_FragColor = LightColorBasedOnDist*vec4((ambient + diffuse), 1.0);
}

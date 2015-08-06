$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 lightPositionViewSpace_0;
uniform float lightIntensity_0;
uniform float lightAttenuation_0;
uniform float lightRadius_0;
uniform vec3 lightDiffuseColor_0;
uniform vec3 lightSpecularColor_0;
varying vec3 lightVectorTangentSpace_0;
varying float intensityBasedOnDist_0;
uniform vec4 lightPositionViewSpace_1;
uniform float lightIntensity_1;
uniform float lightAttenuation_1;
uniform float lightRadius_1;
uniform vec3 lightDiffuseColor_1;
uniform vec3 lightSpecularColor_1;
varying vec3 lightVectorTangentSpace_1;
varying float intensityBasedOnDist_1;
uniform vec4 lightPositionViewSpace_2;
uniform float lightIntensity_2;
uniform float lightAttenuation_2;
uniform float lightRadius_2;
uniform vec3 lightDiffuseColor_2;
uniform vec3 lightSpecularColor_2;
varying vec3 lightVectorTangentSpace_2;
varying float intensityBasedOnDist_2;
uniform vec4 lightPositionViewSpace_3;
uniform float lightIntensity_3;
uniform float lightAttenuation_3;
uniform float lightRadius_3;
uniform vec3 lightDiffuseColor_3;
uniform vec3 lightSpecularColor_3;
varying vec3 lightVectorTangentSpace_3;
varying float intensityBasedOnDist_3;
varying vec4 texCoordVarying;
varying vec3 surfaceToCameraTangentSpace;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

void main() {
	vec4 posViewSpace = ModelViewMatrix*Position;
	float lightDistance = 0.0;
	texCoordVarying = TexCoord;
	vec3 vertexNormal_ViewSpace = mat3(ModelViewMatrix) * Normal;
	vec3 vertexTangent_ViewSpace = mat3(ModelViewMatrix) * Tangent;
	vec3 vertexBitangent_ViewSpace = mat3(ModelViewMatrix) * Bitangent;
	vec3 firstRow = vec3(vertexTangent_ViewSpace.x, vertexBitangent_ViewSpace.x, vertexNormal_ViewSpace.x);
	vec3 secondRow = vec3(vertexTangent_ViewSpace.y, vertexBitangent_ViewSpace.y, vertexNormal_ViewSpace.y);
	vec3 thirdRow = vec3(vertexTangent_ViewSpace.z, vertexBitangent_ViewSpace.z, vertexNormal_ViewSpace.z);
	mat3 TBN = mat3(firstRow, secondRow, thirdRow);
	surfaceToCameraTangentSpace = TBN*( - posViewSpace.xyz );
	lightVectorTangentSpace_0 = TBN*(lightPositionViewSpace_0.xyz - posViewSpace.xyz);
	lightDistance = distance(posViewSpace, lightPositionViewSpace_0);
	intensityBasedOnDist_0 = 0.0;
	if (lightDistance <= lightRadius_0) {
	intensityBasedOnDist_0 = clamp(lightIntensity_0 / (lightAttenuation_0*lightDistance*lightDistance), 0.0, 1.0);
	};
	lightVectorTangentSpace_1 = TBN*(lightPositionViewSpace_1.xyz - posViewSpace.xyz);
	lightDistance = distance(posViewSpace, lightPositionViewSpace_1);
	intensityBasedOnDist_1 = 0.0;
	if (lightDistance <= lightRadius_1) {
	intensityBasedOnDist_1 = clamp(lightIntensity_1 / (lightAttenuation_1*lightDistance*lightDistance), 0.0, 1.0);
	};
	lightVectorTangentSpace_2 = TBN*(lightPositionViewSpace_2.xyz - posViewSpace.xyz);
	lightDistance = distance(posViewSpace, lightPositionViewSpace_2);
	intensityBasedOnDist_2 = 0.0;
	if (lightDistance <= lightRadius_2) {
	intensityBasedOnDist_2 = clamp(lightIntensity_2 / (lightAttenuation_2*lightDistance*lightDistance), 0.0, 1.0);
	};
	lightVectorTangentSpace_3 = TBN*(lightPositionViewSpace_3.xyz - posViewSpace.xyz);
	lightDistance = distance(posViewSpace, lightPositionViewSpace_3);
	intensityBasedOnDist_3 = 0.0;
	if (lightDistance <= lightRadius_3) {
	intensityBasedOnDist_3 = clamp(lightIntensity_3 / (lightAttenuation_3*lightDistance*lightDistance), 0.0, 1.0);
	};
	gl_Position = ProjectionMatrix*posViewSpace;
}

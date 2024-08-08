#version 330

/*
  Filename: GeneralShader.vert
  Authors: Gary M. Zoppetti, Ph.D. & Chad Hogg & Marshall Feng
  Description: A vertex shader that demonstrates how up to 8 light sources of a
    variety of types could be provided.
*/

// By default, all float variables will use high precision.
precision highp float;

// The C++ code will tell us how many light sources there are (maximum 8).
uniform int uNumLights;

// Information about one light source.
// Because different light sources store different information, not every type
//   will use every data member.
struct Light
{
  // 0 if directional, 1 if point, 2 if spot -- other values illegal.
  int type;

  // All lights have these parameters.
  vec3 diffuseIntensity;
  vec3 specularIntensity;

  // Point and spot light parameters.
  vec3 position;
  vec3 attenuationCoefficients;

  // Directional and spot light parameter.
  vec3 direction;

  // Spot light parameters.
  float cutoffCosAngle;
  float falloff;
};

// An array of lights that will be filled by the C++ code.
const int MAX_LIGHTS = 16;
uniform Light uLights[MAX_LIGHTS];

// Single ambient light, provided by the C++ code.
uniform vec3  uAmbientIntensity;

// Material properties, provided by the C++ code.
uniform vec3  uAmbientReflection;
uniform vec3  uDiffuseReflection;
uniform vec3  uSpecularReflection;
uniform float uSpecularPower;
uniform vec3  uEmissiveIntensity;

// Inputs from the VBO.
in vec3 aPosition;
in vec3 aNormal;
in vec3 aColor;

// Output to the fragment shader.
out vec3 vColor;

// Transformation matrices, provided by C++ code.
uniform mat4 uView;
uniform mat4 uProjection;
//uniform mat4 uViewProjection;
uniform mat4 uWorld;

// Eye position, in world space, provided by C++ code.
uniform vec3 uEyePosition;

// **

// Calculate diffuse and specular lighting for a single light.
vec3
calculateLighting(Light light, vec3 vertexPosition, vec3 vertexNormal);

// **

void
main (void)
{
  mat4 worldViewProjection = uProjection * uView * uWorld;
  // Transform vertex into clip space

  gl_Position = worldViewProjection * vec4(aPosition, 1);

  if (uNumLights == 0)
  {
    // use the vertex color if not light exist
    vColor = aColor;
    return;
  }

  // Transform vertex into world space for lighting
  vec3 positionWorld = vec3(uWorld * vec4(aPosition, 1));

  // We're doing lighting in world space for this example!
  mat3 normalTransform = mat3(uWorld);
  normalTransform = transpose(inverse(normalTransform));
  // Normal matrix is world inverse transpose
  vec3 normalWorld = normalize(normalTransform * aNormal);

  // Handle ambient and emissive light
  //   It's independent of any particular light
  vColor = uAmbientReflection * uAmbientIntensity + uEmissiveIntensity;
  // Iterate over all lights and calculate diffuse and specular contributions
  for (int i = 0; i < uNumLights; ++i)
  {
    vColor += calculateLighting(uLights[i], positionWorld, normalWorld);
  }
  // Stay in bounds [0, 1]
  vColor = clamp(vColor, 0.0, 1.0);

  vColor *= aColor;  // Combine vertex color with lighting
}

// **

vec3
calculateLighting (Light light, vec3 vertexPosition, vec3 vertexNormal)
{
  // Light vector points toward the light
  vec3 lightVector;
  if (light.type == 0)
  {
    // Directional
    lightVector = normalize(-light.direction);
  }
  else
  {
    // Point or spot
    lightVector = normalize(light.position - vertexPosition);
  }
  // Light intensity is proportional to angle between light vector
  //   and vertex normal
  float lambertianCoef = max(dot(lightVector, vertexNormal), 0.0);
  vec3 diffuseAndSpecular = vec3(0.0);
  if (lambertianCoef > 0.0)
  {
    // Light is incident on vertex, not shining on its edge or back
    vec3 diffuseColor = uDiffuseReflection * light.diffuseIntensity;
    diffuseColor *= lambertianCoef;

    vec3 specularColor = uSpecularReflection * light.specularIntensity;
    // See how light reflects off of vertex
    vec3 reflectionVector = reflect(-lightVector, vertexNormal);
    // Compute view vector, which points toward the eye
    vec3 eyeVector = normalize(uEyePosition - vertexPosition);
    // Light intensity is proportional to angle between reflection vector
    //   and eye vector
    float specularCoef = max(dot(eyeVector, reflectionVector), 0.0);
    // Material's specular power determines size of bright spots
    specularColor *= pow(specularCoef, uSpecularPower);

    float attenuation = 1.0;
    if (light.type != 0)
    { // Non-directional, so light attenuates
      float distance = length(vertexPosition - light.position);
      attenuation = 1.0 / (
            light.attenuationCoefficients.x
          + light.attenuationCoefficients.y * distance
          + light.attenuationCoefficients.z * distance * distance);
    }
    float spotFactor = 1.0f;
    if (light.type == 2)
    { // Spot light
      float cosTheta = dot(-lightVector, light.direction);
      cosTheta = max(cosTheta, 0.0f);
      spotFactor = (cosTheta >= light.cutoffCosAngle) ? cosTheta : 0.0f;
      spotFactor = pow(spotFactor, light.falloff);
    }
    diffuseAndSpecular = spotFactor * attenuation * (diffuseColor + specularColor);
  }

  return diffuseAndSpecular;
}

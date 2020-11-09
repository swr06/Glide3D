#version 330 core

#define PI 3.14159265359
#define G_SCATTERING 0.75f
#define NB_STEPS 100

layout(location = 0) out vec3 o_VolumetricFog; // outputs to the volumetric texture that is in half resolution
in vec2 v_TexCoords;

uniform sampler2D u_PositionTexture;
uniform sampler2D u_ShadowMap;
uniform mat4 u_LightViewProjection;
uniform vec3 u_ViewerPosition;
uniform vec3 u_LightDirection;

// Henyey-Greenstein phase function
float ComputeScattering(float lightDotView) // Dot product of the light direction vector and the view vector
{
	float result = 1.0f - G_SCATTERING * G_SCATTERING;
	result /= (4.0f * PI * pow(1.0f + G_SCATTERING * G_SCATTERING - (2.0f * G_SCATTERING) * lightDotView, 1.5f));
	
	return result;
}

float ShadowCalculation(vec3 Position)
{
	vec4 FragPosLightSpace = u_LightViewProjection * vec4(Position, 1.0f);
    vec3 ProjectionCoordinates = FragPosLightSpace.xyz / FragPosLightSpace.w;
    ProjectionCoordinates = ProjectionCoordinates * 0.5 + 0.5;
    float SampledDepth = texture(u_ShadowMap, ProjectionCoordinates.xy).r; 
    float CurrentDepth = ProjectionCoordinates.z;
    float bias = 0.005f;
	float shadow = CurrentDepth - bias > SampledDepth  ? 1.0 : 0.0;  

    return shadow;
}

void main()
{
	vec3 WorldPosition = texture(u_PositionTexture, v_TexCoords).xyz;
	vec3 StartPosition = u_ViewerPosition;
	vec3 EndRayPosition = WorldPosition; 

	vec3 RayVector = EndRayPosition.xyz - StartPosition;
	float RayLength = length(RayVector);
	vec3 RayDirection = RayVector / RayLength;

	float StepLength = RayLength / NB_STEPS;

	vec3 step_sz = RayDirection * StepLength;

	vec3 CurrentPosition = StartPosition;

	vec3 TotalFog = vec3(0.0f);

	for (int i = 0; i < NB_STEPS; i++)
	{
		// Convert to clip space
		vec4 ShadowMapCoords = u_LightViewProjection * vec4(CurrentPosition, 1.0f);

		// Perspective Divide
		ShadowMapCoords.xyz /= ShadowMapCoords.w;

		// Sample Depth
		float ShadowDepth = texture(u_ShadowMap, ShadowMapCoords.xy).r;
		const float bias = 0.005f;

		if (ShadowCalculation(CurrentPosition) == 0.0f)
		{
			TotalFog += ComputeScattering(dot(RayDirection, -u_LightDirection)) * vec3(1.0f);
		}

		CurrentPosition += step_sz;
	}

	TotalFog /= NB_STEPS;
	o_VolumetricFog = TotalFog;
}
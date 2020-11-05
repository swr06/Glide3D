#version 330 core

#define PI 3.14159265359
#define G_SCATTERING 0.1f
#define NB_STEPS 50

layout(location = 0) out vec3 o_VolumetricFog; // outputs to the volumetric texture that is in half resolution
in vec2 v_TexCoords;

uniform sampler2D u_DepthTexture;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_ShadowMap;
uniform mat4 u_LightViewProjection;
uniform vec3 u_ViewerPosition;
uniform vec3 u_LightDirection;

// Henyey-Greenstein phase function
float ComputeScattering(float lightDotView)
{
	float result = 1.0f - G_SCATTERING * G_SCATTERING;
	result /= (4.0f * PI * pow(1.0f + G_SCATTERING * G_SCATTERING - (2.0f * G_SCATTERING) * lightDotView, 1.5f));
	
	return result;
}

void main()
{
	vec3 WorldPosition = texture(u_PositionTexture, v_TexCoords).xyz;
	vec3 StartPosition = u_ViewerPosition;
	vec3 EndRayPosition = WorldPosition; // Todo!

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

		float ShadowDepth = texture(u_ShadowMap, ShadowMapCoords.xy).r;

		if (ShadowDepth > ShadowMapCoords.z)
		{
			TotalFog += ComputeScattering(dot(RayDirection, u_LightDirection)) * vec3(1.0f);
		}

		CurrentPosition += step_sz;
	}

	TotalFog /= NB_STEPS;
	o_VolumetricFog = TotalFog;
}
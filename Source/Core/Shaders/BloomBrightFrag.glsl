#version 330 core

layout (location = 0) out vec3 o_Color; // The bright fragments of the scene

in vec2 v_TextureCoordinates;
uniform sampler2D u_Texture;

void main()
{
    vec3 Color = texture(u_Texture, v_TextureCoordinates).rgb;
	float Brightness = (Color.r * 0.2126f) + (Color.g * 0.7152f) + (Color.b * 0.722f);

    // We want to avoid branching in shaders 
    o_Color = mix(vec3(0.0f), Color, float(Brightness > 0.5f));
}
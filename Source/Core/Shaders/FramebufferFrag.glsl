#version 330 core

in vec2 v_TexCoords;
out vec4 o_Color;

uniform sampler2D u_FramebufferTexture;

void main()
{
	o_Color = texture(u_FramebufferTexture, v_TexCoords);
	//o_Color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
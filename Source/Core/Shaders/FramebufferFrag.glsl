#version 330 core

in vec2 v_TexCoords;
layout(location = 0) out vec4 o_Color;

uniform sampler2D u_FramebufferTexture;

void main()
{
	o_Color = texture(u_FramebufferTexture, v_TexCoords);
	o_Color = pow(o_Color.rgba, vec4(1.0/2.2f));
}
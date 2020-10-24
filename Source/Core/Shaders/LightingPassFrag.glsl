#version 330 core

layout (location = 0) out vec4 o_Color;

in vec2 v_TextureCoordinates;

uniform sampler2D u_PositionTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_ColorTexture;

void main()
{
	o_Color = vec4(texture(u_PositionTexture, v_TextureCoordinates));
}

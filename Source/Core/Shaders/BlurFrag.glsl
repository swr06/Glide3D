#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 v_TexCoords;
uniform sampler2D u_Texture;

void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(u_Texture, 0));
    vec4 result = vec4(0.0f);

    for (int x = -2; x < 2; x++) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(u_Texture, v_TexCoords + offset).r;
        }
    }

    FragColor = result / (4.0 * 4.0);
}  
#version 330 core
layout (location = 0) out vec3 o_Color;
  
in vec2 v_TexCoords;

uniform sampler2D u_Texture;
  
float Weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(u_Texture, 0); 
    vec3 result = texture(u_Texture, v_TexCoords).rgb * Weights[0]; 

    for(int i = 1; i < 5; i++)
    {
        result += texture(u_Texture, v_TexCoords + vec2(0.0, tex_offset.y * i)).rgb * Weights[i];
        result += texture(u_Texture, v_TexCoords - vec2(0.0, tex_offset.y * i)).rgb * Weights[i];
    }

    o_Color = result;
}
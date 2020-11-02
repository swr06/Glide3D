#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out; // max vertices = 3 * 6 (per cubemap face) = 18

uniform mat4 u_ViewProjectionMatrices[6];

out vec4 gs_FragPosition;

void main()
{
    for(int face = 0; face < 6; face++)
    {
        gl_Layer = face; 

        for(int i = 0; i < 3; i++) 
        {
            gs_FragPosition = gl_in[i].gl_Position; // gl_Position is in world space
            gl_Position = u_ViewProjectionMatrices[face] * gl_in[i].gl_Position;
            EmitVertex();
        }    

        EndPrimitive();
    }
} 
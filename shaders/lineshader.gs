#version 410 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float scaling;

vec3 GetNormal(int index)
{
   float dx = gl_in[index + 1].gl_Position.x - gl_in[index].gl_Position.x;
   float dy = gl_in[index + 1].gl_Position.y - gl_in[index].gl_Position.y;
   vec3  npos = vec3(-1 * dy, dx, 0.0f);
   return normalize(npos) * scaling;
}

void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(GetNormal(0), 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position - vec4(GetNormal(0), 0.0); 
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + vec4(GetNormal(0), 0.0); 
    EmitVertex();
    gl_Position = gl_in[1].gl_Position - vec4(GetNormal(0), 0.0); 
    EmitVertex();

    EndPrimitive();
}

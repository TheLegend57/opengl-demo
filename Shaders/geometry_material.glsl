#version 330 core

layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 3) out;

in vec3 vs_position[];

out vec3 gs_position;
out vec3 gs_normal;

void main() {
    for (int i = 0; i < 3; ++i){
        gs_position = vs_position[i];
        gs_normal = normalize(cross(vs_position[1] - vs_position[0], vs_position[2] - vs_position[0]));
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}

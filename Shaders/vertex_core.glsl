#version 330 core
layout (location = 0) in vec3 vertex_position;

out vec3 vs_position;
out vec3 vs_texcoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){
    vs_position = (ModelMatrix * vec4(vertex_position, 1.f)).xyz;
    vs_texcoord.x = vertex_texcoord.x;
    vs_texcoord.y = vertex_texcoord.y * -1.0f;
    vs_texcoord.z = vertex_texcoord.z;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
}

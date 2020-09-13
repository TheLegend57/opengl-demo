#version 330 core
layout (location = 0) in vec3 vertex_position;

out vec3 vs_position;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){
    vs_position = (ModelMatrix * vec4(vertex_position, 1.f)).xyz;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
}

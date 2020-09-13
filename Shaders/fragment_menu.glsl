#version 330 core
in vec2 vs_texcoord;
out vec4 fs_color;
uniform sampler2D menuTexture;
void main(){
	fs_color = texture2D(menuTexture, vs_texcoord);
}
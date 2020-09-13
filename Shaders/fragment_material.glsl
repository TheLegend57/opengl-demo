#version 330 core

struct Material{
	float SpecularExponent;
	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;
	vec3 EmissionColor;
	float OpticalDensity;
	float Transparency;
};

struct DirectionalLight{
	vec3 Direction;
	float Strength;
	vec3 LightColor;
};

in vec3 gs_position;
in vec3 gs_normal;

out vec4 fs_color;

uniform Material material;
uniform DirectionalLight light;
uniform vec3 CameraPosition;

vec3 calculateAmbient(){
    return light.Strength * light.LightColor * material.AmbientColor;
}

vec3 calculateDiffuse(){
    return light.Strength * light.LightColor * material.DiffuseColor * (max(0, dot(-light.Direction, gs_normal))*0.7f + 0.3f);
}

vec3 calculateSpecular(){
	vec3 ReflectionVector = normalize(2*cross(cross(-light.Direction, gs_normal), gs_normal) + light.Direction);
	return light.Strength*max(vec3(0.f, 0.f, 0.f), cross(light.Direction, gs_normal))*pow(max(0, dot(ReflectionVector, (gs_position - CameraPosition))), material.SpecularExponent) * light.LightColor * material.SpecularColor;
}

void main(){
    vec3 ambientFinal = calculateAmbient();
    vec3 diffuseFinal = calculateDiffuse();
    vec3 specularFinal = calculateSpecular();
    fs_color = vec4(diffuseFinal, 1.f - material.Transparency);
    //Something is wrong. Do not care enough to fix it
    //fs_color = vec4(ambientFinal + diffuseFinal + specularFinal, 1.f - material.Transparency);
}

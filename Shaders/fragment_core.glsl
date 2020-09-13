#version 330 core

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    samplerCube diffuseTex;
    samplerCube specularTex;
    //sampler2D diffuseTex;
    //sampler2D specularTex;
};

in vec3 gs_position;
in vec3 gs_normal;
in vec3 gs_texcoord;

out vec4 fs_color;

uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

vec4 calculateAmbient(){
    return vec4(material.ambient * 0.5f + vec3(0.3f, 0.3f, 0.3f), 1.f);
}

vec4 calculateDiffuse(){
    vec3 posToLightDirVec = normalize(lightPos0 - gs_position);
    float diffuse = clamp(dot(posToLightDirVec, gs_normal), 0, 1);
    return vec4(material.diffuse * diffuse, 1.f);
}

vec4 calculateSpecular(){
    vec3 lightToPosDirVec = normalize(gs_position - lightPos0);
    vec3 reflectedDirVec = normalize(reflect(lightToPosDirVec, normalize(gs_normal)));
    vec3 posToViewDirVec = normalize(cameraPos - gs_position);
    float specularConstant = pow(max(dot(posToViewDirVec, reflectedDirVec), 0), 30);
    return vec4(material.specular * specularConstant * texture(material.specularTex, gs_texcoord).rgb, 1.f);
}

void main(){
    vec4 ambientFinal = calculateAmbient();
    vec4 diffuseFinal = calculateDiffuse();
    vec4 specularFinal = calculateSpecular();
    //Attenuation

    //Final light
    
    fs_color = texture(material.diffuseTex, gs_texcoord)*(ambientFinal + diffuseFinal + specularFinal);
}

#version 330 core
in vec3 Position;
in vec3 Normal;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D dudvTexture;
void main()
{
    vec4 reflectColor;
    vec4 refractColor;
    vec3 I = normalize(Position - cameraPos);
    vec3 RL = reflect(I, Normal);
    vec3 RR = refract(I, Normal, 0.9);
    reflectColor = texture(skybox, RL);
    refractColor = texture(skybox, RR);
    float viewangle = dot(Normal, -I);
    color = (1-viewangle)*reflectColor + viewangle*refractColor;

}

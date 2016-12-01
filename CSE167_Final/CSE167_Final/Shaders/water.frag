#version 330 core
in vec3 Position;
in vec3 Normal;
in vec2 textureCoords;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D dudvTexture;
uniform float move;
void main()
{
    vec2 distortion = texture(dudvTexture, vec2(textureCoords.x + move,textureCoords.y)).rg * 2.0 - 1.0;
    vec2 distortionAgain = texture(dudvTexture, vec2(move - textureCoords.x,textureCoords.y + move)).rg * 2.0 - 1.0;
    distortion += distortionAgain;
    vec4 reflectColor;
    vec4 refractColor;
    vec3 I = normalize(Position - cameraPos);
    vec3 RL = reflect(I, Normal);
    vec3 RR = refract(I, Normal, 0.9);
    RL += vec3(distortion.x, distortion.y, 0.0);
    RR += vec3(distortion.x, distortion.y, 0.0);
    reflectColor = texture(skybox, RL);
    refractColor = texture(skybox, RR);
    float viewangle = dot(Normal, -I);
    color = (1-viewangle)*reflectColor + viewangle*refractColor;
    color = mix(color, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
}

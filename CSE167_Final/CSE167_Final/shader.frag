#version 330 core
in vec3 Normal;
in vec3 Position;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    color = texture(skybox, R);
    vec4 temp = vec4(Normal.x/10.0, Normal.y/10.0, Normal.z/10.0, 1.0f);
    color = mix(color, temp, 0.4);
}

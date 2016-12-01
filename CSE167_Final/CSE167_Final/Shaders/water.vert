#version 330 core
layout (location = 0) in vec3 position;

out vec3 Position;
out vec3 Normal;
out vec2 textureCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
const float tiling = 6.0;
void main()
{
    vec3 normal = vec3(0.0f, 1.0f, 0.0f);
    gl_Position = projection * view * model * vec4(position, 1.0f);
    Normal = mat3(transpose(inverse(model))) * normal;
    Normal = normalize(Normal);
    Position = vec3(model * vec4(position, 1.0f));
    textureCoords = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5) * tiling;
}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 b;
layout (location = 2) in vec3 c;

out vec3 TexCoords;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * transform * vec4(aPos, 1.0);
    gl_Position = pos;
}
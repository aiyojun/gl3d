#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 fragColor;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    fragColor = aColor;
}
// --
#version 330 core

in vec3 fragColor;

void main()
{
    gl_FragColor = vec4(fragColor, 1.0);
}
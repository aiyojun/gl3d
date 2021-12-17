#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

// --
#version 330 core

in vec3 normal;
in vec2 TexCoords;

uniform sampler2D tex;

void main()
{
    gl_FragColor  = texture(tex, TexCoords);
//    gl_FragColor  = vec4(0.6, 0.6, 0.6, 1.0);
}
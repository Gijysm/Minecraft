#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aLight;

out vec2 TexCoord;
out float Light;

uniform mat4 model;
uniform mat4 projview;

void main()
{
    gl_Position = projview * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Light = aLight;
}

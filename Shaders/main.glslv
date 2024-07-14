#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float vLight;

out vec4 a_Color;
out vec2 a_texCoord;

uniform mat4 model;
uniform mat4 projview;

void main()
{
a_Color = vec4(vLight,vLight,vLight, 1.0);
    a_texCoord = aTexCoord;
    gl_Position = projview * model * vec4(aPos, 1.0);
}

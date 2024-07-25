#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec4 vLight;

out vec2 TexCoord;
out vec4 Color;

uniform mat4 model;
uniform mat4 projview;

void main()
{
    Color = vec4(vLight.r, vLight.g, vLight.b, 1.0);
    TexCoord = vTexCoord;
    Color.rgb += vLight.a;
    gl_Position = projview * model * vec4(vPos, 1.0);
}

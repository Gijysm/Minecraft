#version 330 core

in vec4 a_Color;
in vec2 a_texCoord;
out vec4 f_color;

uniform sampler2D u_texture0;

void main()
{
    f_color = a_Color * texture(u_texture0, a_texCoord);
}

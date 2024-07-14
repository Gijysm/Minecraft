#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float Light;

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    FragColor = vec4(texColor.rgb * Light, texColor.a);
}

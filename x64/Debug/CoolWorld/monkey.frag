#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D mytexture;

void main()
{
    vec4 result = texture2D(mytexture, texCoord);

    FragColor = result;
} 
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform int inspectorSelect;

out vec2 texCoord;
out flat int inspectorSelectOut;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
void main()
{
    mat4 mvp = projection*view*model;
    gl_Position = mvp * vec4(aPos.x,aPos.y, aPos.z, 1.0);

    inspectorSelectOut = inspectorSelect;

    texCoord = aTexCoord;
}
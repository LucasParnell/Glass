#version 430 core

out vec4 FragColor;

in vec2 texCoord;
flat in int inspectorSelectOut;

uniform sampler2D mytexture;



void main()
{

    vec4 result = texture2D(mytexture, texCoord);

    if(inspectorSelectOut == 1){
        result += vec4(0.0,0.5,0.0,0.0);
    }
    
 


    FragColor = result;
} 
#version 330 core

in vec2 texCoord;

out vec4 FragColor;

//uniform vec4 lightColor;
uniform sampler2D tex2;

void main()
{
	FragColor = texture(tex2, texCoord); // *lightColor (ale zewnetrzny kolor a nie swoj)
}
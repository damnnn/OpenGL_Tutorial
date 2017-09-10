#version 330 core

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, ourTexCoord), texture(texture2, ourTexCoord), texture(texture2, ourTexCoord).a * 0.2);
	//FragColor = vec4(ourColor, 1.0);
}
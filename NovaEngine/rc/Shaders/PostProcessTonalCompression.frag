#version 460 core

in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D image;
uniform float exposure;

void main()
{
	vec3 sceneColor = texture(image, textureCoordinates).rgb;
	sceneColor = vec3(1.0) - exp(-sceneColor * exposure);
	FragColor = vec4(sceneColor, 1.0);
}
#version 460 core

in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D image;
uniform float gamma;

void main()
{
	vec3 sceneColor = texture(image, textureCoordinates).rgb;
	//sceneColor = pow(sceneColor, vec3(gamma));

	sceneColor.x = sceneColor.x <= 0.0031308 ? sceneColor.x / 12.92 : pow(sceneColor.x, 1.0 / 2.4) * 1.055 - 0.055;
	sceneColor.y = sceneColor.y <= 0.0031308 ? sceneColor.y / 12.92 : pow(sceneColor.y, 1.0 / 2.4) * 1.055 - 0.055;
	sceneColor.z = sceneColor.z <= 0.0031308 ? sceneColor.z / 12.92 : pow(sceneColor.z, 1.0 / 2.4) * 1.055 - 0.055;

	FragColor = vec4(sceneColor, 1.0);
}
#version 460 core

in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D image;
uniform float sharpness;

void main()
{
	vec2 texSize   = textureSize(image, 0).xy;

	float neighbor = sharpness * -1;
	float center   = sharpness * 4 + 1;

	vec3 color =
	  texture(image, vec2(gl_FragCoord.x + 0, gl_FragCoord.y + 1) / texSize).rgb
	* neighbor

	+ texture(image, vec2(gl_FragCoord.x - 1, gl_FragCoord.y + 0) / texSize).rgb
	* neighbor
	+ texture(image, vec2(gl_FragCoord.x + 0, gl_FragCoord.y + 0) / texSize).rgb
	* center
	+ texture(image, vec2(gl_FragCoord.x + 1, gl_FragCoord.y + 0) / texSize).rgb
	* neighbor

	+ texture(image, vec2(gl_FragCoord.x + 0, gl_FragCoord.y - 1) / texSize).rgb
	* neighbor
	;

	FragColor = vec4(color, texture(image, textureCoordinates).a);
}
#version 460 core

in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D image;
uniform mat3 kernelMatrix;
uniform float offset;

void main()
{
	vec2 vecOffset = textureSize(image, 0) * offset;

	vec2 offsets[9] = vec2[](
	    vec2(-vecOffset.y,  vecOffset.x),
	    vec2( 0.0f,			vecOffset.x),
	    vec2( vecOffset.y,  vecOffset.x),
	    vec2(-vecOffset.y,  0.0f),
	    vec2( 0.0f,			0.0f),
	    vec2( vecOffset.y,  0.0f),
	    vec2(-vecOffset.y,  -vecOffset.x),
	    vec2( 0.0f,			-vecOffset.x),
	    vec2( vecOffset.y,  -vecOffset.x)
	);

	float kernel[9] = float[](
	    kernelMatrix[0][0], kernelMatrix[1][0], kernelMatrix[2][0],
	    kernelMatrix[0][1], kernelMatrix[1][1], kernelMatrix[2][1],
	    kernelMatrix[0][2], kernelMatrix[1][2], kernelMatrix[2][2]
	);

	vec3 texelWithOffset;
	vec3 color = vec3(0.0);
	for(int i = 0; i < 9; ++i)
	{
		texelWithOffset = texture(image, textureCoordinates + offsets[i]).xyz;
		color += texelWithOffset * kernel[i];
	}

	FragColor = vec4(color, 1.0);
}
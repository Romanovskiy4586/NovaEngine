#version 460 core
		
in vec3 aPosition;
in vec2 aTextureCoordinates;

out vec2 textureCoordinates;

uniform mat4 pvm;

layout (std430) readonly buffer PositionOffsetsBuffer
{
    vec2 positionOffsets[];
};
layout (std430) readonly buffer UVOffsetsBuffer
{
    vec2 uvOffsets[];
};

void main()
{
    vec2 offset = positionOffsets[gl_InstanceID];
    gl_Position = pvm * vec4(aPosition.xy + offset, aPosition.z, 1.0);
    textureCoordinates = uvOffsets[gl_InstanceID];
}
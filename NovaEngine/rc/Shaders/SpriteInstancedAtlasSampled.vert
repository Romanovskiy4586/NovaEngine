#version 460 core
		
in vec3 aPosition;
in vec2 aTextureCoordinates;

out vec2 textureCoordinates;

uniform mat4 pvm;
uniform vec2 atlasSize;// 256, 256
uniform vec2 tileSize; // 16, 16
uniform vec2 tileIndex;

layout (std430) readonly buffer PositionOffsetsBuffer
{
    vec2 positionOffsets[];
};

void main()
{
    vec2 offset = positionOffsets[gl_InstanceID];
    gl_Position = pvm * vec4(aPosition.xy + offset, aPosition.z, 1.0);

    vec2 tileSizeInUV = tileSize / atlasSize;
    vec2 uvOffset = tileIndex * tileSizeInUV;
    textureCoordinates = aTextureCoordinates * tileSizeInUV + uvOffset;
}
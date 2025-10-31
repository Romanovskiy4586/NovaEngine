#version 460 core
		
in vec3 aPosition;
in vec2 aTextureCoordinates;

out vec2 textureCoordinates;

uniform mat4 pvm;
uniform vec2 atlasSize;
uniform vec2 tileSize;
uniform vec2 tileIndex;

layout (std430) readonly buffer PositionOffsetsBuffer
{
    vec2 positionOffsets[];
};

void main()
{
    vec2 offset = positionOffsets[gl_InstanceID];
    gl_Position = pvm * vec4(aPosition.xy + offset, aPosition.z, 1.0);

    const vec2 shrinkingSize = tileSize * 0.0000;
    vec2 pixelSizeInUV = 1 / atlasSize;
    vec2 tileSizeInUV = tileSize / atlasSize;
    vec2 uvOffset = tileIndex * tileSizeInUV;

    vec2 shrinkingOffset = aTextureCoordinates * (-shrinkingSize * 2);
    textureCoordinates = aTextureCoordinates * tileSizeInUV + uvOffset + shrinkingSize + shrinkingOffset;
}
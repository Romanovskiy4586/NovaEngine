#version 460 core
		
in vec3 aPosition;
in vec2 aTextureCoordinates;

out vec2 textureCoordinates;

uniform mat4 pvm;
uniform vec2 atlasSize;
uniform vec2 tileSize;

layout (std430) readonly buffer GlyphsIndicesBuffer
{
    vec2 glyphIndices[];
};

void main()
{
    vec2 currentGlyphIndex = glyphIndices[gl_InstanceID];
    float offset = gl_InstanceID * tileSize.x * 0.1;
    gl_Position = pvm * vec4(aPosition.x + offset, aPosition.y, aPosition.z, 1.0);

    vec2 pixelSizeInUV = 1 / atlasSize;
    vec2 tileSizeInUV = tileSize / atlasSize;
    vec2 uvOffset = currentGlyphIndex * tileSizeInUV;

    textureCoordinates = aTextureCoordinates * tileSizeInUV + uvOffset;
    textureCoordinates += vec2(pixelSizeInUV.x / 2, 0.0);
}
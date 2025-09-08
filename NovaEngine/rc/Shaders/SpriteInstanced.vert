#version 460 core
		
in vec3 aPosition;
in vec2 aTextureCoordinates;

out vec2 textureCoordinates;

uniform mat4 pvm;

layout (std430) readonly buffer OffsetsBuffer
{
    vec2 offsets[];
};  

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = pvm * vec4(aPosition.xy + offset, aPosition.z, 1.0);
    textureCoordinates = aTextureCoordinates;
}
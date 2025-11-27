#version 460 core

out vec4 FragColor;

in vec2 textureCoordinates;
in vec2 screenPixelSize;

uniform sampler2D albedo;

void main()
{
    FragColor = texture(albedo, textureCoordinates);

    if (texture(albedo, textureCoordinates).a == 0)
    {
        vec2 offsets[4] = vec2[](vec2(0, 1), vec2(0, -1), vec2(1, 0), vec2(-1, 0));
        const float thickness = 0.000001;

        for (int i = 0; i < 4; ++i)
        {
            if (texture(albedo, textureCoordinates + offsets[i] * screenPixelSize * thickness) == vec4(0.0, 0.0, 0.0, 1.0))
            {
                FragColor = vec4(1.0, 1.0, 1.0, 1.0);
            }
        }
    }
}
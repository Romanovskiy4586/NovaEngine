export module ShadersSourceCodeDB;
#include "Core.h"
import std;
import NSL;

export namespace Renderer
{
	struct NSL_API ShadersSourceCodeDB final
	{
		const std::string positionUVsPVMVertexShader =
		R"(
		#version 460 core
		
		in vec3 aPosition;
		in vec2 aTextureCoordinates;
		
		out vec2 textureCoordinates;
		
		uniform mat4 pvm;
		
		void main()
		{
		    gl_Position = pvm * vec4(aPosition, 1.0);
		    textureCoordinates = aTextureCoordinates;
		}
		)";

		const std::string positionUVsPVMPS1VertexShader =
		R"(
		#version 460 core
		
		in vec3 aPosition;
		in vec2 aTextureCoordinates;
		
		out vec2 textureCoordinates;
		
		uniform mat4 pvm;
		
		void main()
		{
		    const int stabilityLevel = 1000;
		
			gl_Position = pvm * vec4(aPosition, 1.0);
			gl_Position.x = int(gl_Position.x * stabilityLevel) / float(stabilityLevel);
		    gl_Position.y = int(gl_Position.y * stabilityLevel) / float(stabilityLevel);
		    gl_Position.z = int(gl_Position.z * stabilityLevel) / float(stabilityLevel);

		    textureCoordinates = aTextureCoordinates;
		}
		)";

		const std::string textureMappingFragmentShader =
		R"(
		#version 460 core
		
		out vec4 FragColor;
		
		in vec2 textureCoordinates;
		
		uniform sampler2D albedo;
		
		void main()
		{
		    FragColor = texture(albedo, textureCoordinates);
		}
		)";

		const std::string postProcessVertexShader =
		R"(
		#version 460 core
		
		in vec2 aPosition;
		in vec2 aTextureCoordinates;
		
		out vec2 textureCoordinates;
		
		void main()
		{
		    gl_Position = vec4(aPosition, 0.0, 1.0);
		    textureCoordinates = aTextureCoordinates;
		}
		)";

		const std::string postProcessFragmentShader_Empty =
		R"(
		#version 460 core
		
		in vec2 textureCoordinates;
		
		out vec4 FragColor;
		
		uniform sampler2D image;
		
		void main()
		{
			FragColor = texture(image, textureCoordinates);
		}
		)";

		const std::string postProcessFragmentShader_GammaCorrection =
		R"(
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
		)";

		const std::string postProcessFragmentShader_TonalCompression =
		R"(
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
		)";

		const std::string postProcessFragmentShader_Sharpener =
		R"(
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
		)";

		const std::string postProcessFragmentShader_Kernel =
		R"(
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
		)";

		const std::string spriteBitmapVertexShader =
		R"(
		#version 460 core
		
		in vec2 aPosition;
		
		uniform mat4 pvm;
		
		void main()
		{
			gl_Position = pvm * vec4(aPosition, 0.0, 1.0);
		}
		)";

		const std::string spriteBitmapFragmentShader =
		R"(
		#version 460 core
		
		out vec4 FragColor;
		
		uniform vec4 color;
		uniform float randomSeed;
		
		float Random(vec2 st)
		{
			return fract(sin(dot(st.xy,
				vec2(12.9898, 78.233))) *
				43758.5453123 * randomSeed);
		}
		void main()
		{
			float randomNumber = Random(gl_FragCoord.xy);
			FragColor = vec4(vec3(randomNumber), 1.0);
		}
		)";
	};
}
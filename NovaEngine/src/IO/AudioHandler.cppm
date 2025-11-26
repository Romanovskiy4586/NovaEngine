export module AudioHandler;
import NSL;
#include "Core.h"

export namespace IO
{
	class NSL_API AudioHandler
	{
	public:
		struct NSL_API Sample
		{
			Sample() NSL_NOEXCEPT
				: bufferID(0)
				, sourceID(0)
			{
			}

			unsigned int bufferID;
			unsigned int sourceID;
		};

	public:
		AudioHandler() NSL_NOEXCEPT
			: _contextPtr(nullptr)
			, _openedDevicePtr(nullptr)
			, _devices()
			, _samples()
		{
			LogInfo("Creating AudioHandler...");
			OpenDefaultDevice();
			LogInfo("AudioHandler initialized");
		}
		~AudioHandler() NSL_NOEXCEPT
		{
			DeleteAllAudioFiles();
			auto device = alcGetContextsDevice(_contextPtr);
			alcMakeContextCurrent(0);
			alcDestroyContext(_contextPtr);
			alcCloseDevice(device);
			_contextPtr = nullptr;
			LogInfo("AudioHandler destroyed. OpenAL context destroyed, used audio device is closed");
		}

	public:
		void LoadWAVE(const std::string& name, const std::string& wavePath, NSL::Vector3 sourcePosition) NSL_NOEXCEPT
		{
			NSL::WAVE wave = NSL::WAVE::Load(wavePath);
			Sample sample;

			ALCall(alGenBuffers(1, &(sample.bufferID)));
			int alFormat;
			switch (wave.format.numChannels)
			{
			case 1:
				alFormat = wave.format.bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
				break;

			case 2:
				alFormat = wave.format.bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
				break;

			default:
				alFormat = AL_FORMAT_MONO8;
			}

			ALCall(alBufferData(static_cast<ALuint>(sample.bufferID), alFormat, static_cast<const ALvoid*>(wave.data.data.c_str()),
				static_cast<ALsizei>(wave.data.data.size()), static_cast<ALsizei>(wave.format.sampleRate)));
			ALCall(alGenSources(1, &(sample.sourceID)));
			ALCall(alSourcei(static_cast<ALuint>(sample.sourceID), AL_BUFFER, static_cast<ALuint>(sample.bufferID)));
			ALCall(alSource3f(static_cast<ALuint>(sample.sourceID), AL_POSITION, sourcePosition.X, sourcePosition.Y, sourcePosition.Z));

			_samples[name] = sample;
		}

		void Play(const std::string& name) NSL_NOEXCEPT
		{
			ALCall(alSourcePlay(_samples[name].sourceID));
		}
		void Pause(const std::string& name) NSL_NOEXCEPT
		{
			ALCall(alSourcePause(_samples[name].sourceID));
		}
		void Stop(const std::string& name) NSL_NOEXCEPT
		{
			ALCall(alSourceStop(_samples[name].sourceID));
		}
		void Rewind(const std::string& name) NSL_NOEXCEPT
		{
			ALCall(alSourceRewind(_samples[name].sourceID));
		}

	private:
		void GetAllDevices() NSL_NOEXCEPT
		{
			std::string devicesString = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
			_devices = NSL::DivideString(devicesString, '\0');
		}
		bool OpenDefaultDevice() NSL_NOEXCEPT
		{
			// Open default device
			if (_openedDevicePtr = alcOpenDevice(0))
			{
				LogInfo("Default Device is opened");
			}
			else
			{
				LogError("Failed to open default device");
				return false;
			}

			// Create context
			if (_contextPtr = alcCreateContext(_openedDevicePtr, 0))
			{
				LogInfo("Audio context is created by using deafult device");
			}
			else
			{
				LogError("Failed to create audio context by using default device");
				return false;
			}

			// Set active context
			bool good = ALCall(alcMakeContextCurrent(_contextPtr));
			if (good)
			{
				LogInfo("Context from default device is maked as current");
			}
			else
			{
				LogError("Failed to make context current from default device");
				return false;
			}

			return true;
		}
		void DeleteAllAudioFiles() NSL_NOEXCEPT
		{
			for (auto& sample : _samples)
			{
				if (sample.second.sourceID) ALCall(alDeleteSources(1, &sample.second.sourceID));
				if (sample.second.bufferID) ALCall(alDeleteBuffers(1, &sample.second.bufferID));
			}
		}

	private:
		ALCcontext* _contextPtr;
		ALCdevice* _openedDevicePtr;
		std::vector<std::string> _devices;
		std::unordered_map<std::string, Sample> _samples;
	};
}
#pragma once
#include<fmod.hpp>
#include "SoundUtils.h"

#include<map>
#include<vector>

//gdpAudioNamespace_Begin

struct Audio
{
	const char* name;
	FMOD::Sound* Audio;
};

struct Channel
{
	const char* name;
	FMOD::Channel* fmodChannel;
	float volume;
	float pitch;
	float pan;
	bool playing;
	bool looping = false;
	bool paused = false;
};

class SoundManager
{
private:
	bool m_Initialized = false;

	std::map<const char*, Audio*> m_AudioMap;
	std::vector<Channel*> m_ChannelList;

	int m_NextChannelId = 0;

	FMOD::System* m_System = nullptr;
	FMOD::Sound* m_Sound = nullptr;
	//FMOD::Channel* m_Channel = nullptr;
	FMOD_RESULT m_Result;

public:
	SoundManager(); 

	~SoundManager();

	void Initialize();
	void Destroy();

	bool GetPauseStatus(int id);
	bool GetLoopStatus(int id);
	float GetChannelVolume(int id);
	float GetChannelPitch(int id);
	float GetChannelPan(int id);

	const char* GetAudioName(int id);

	void LoadSoundStream(const char* source);
	void LoadSound(const char* file);
	int PlaySound(const char* SoundName);
	
	//void SetAudioName(int id);
	void SetChannelVolume(int id, float value);
	void SetChannelPitch(int id, float value);
	void SetChannelPan(int id, float value);
	void PauseSound(int id);
	void StopAudio(int id);
	void LoopAudio(int id);
	
	void InitializeChannelAttributes(int id);
	bool IsChannelPlaying(int id);
	void GetPlaybackPosition(int id, unsigned int& value);
	void GetAudioLength(const char* file, unsigned int& value);

	void Update();

	void PrintInfo() const;
};

//gdpAudioNamespace_End
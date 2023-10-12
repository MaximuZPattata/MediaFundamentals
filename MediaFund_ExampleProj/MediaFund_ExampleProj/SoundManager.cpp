#include "SoundManager.h"
#include<iostream>
#include "SoundUtils.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::Initialize()
{
	if (m_Initialized)
		return;

	FMOD_RESULT result;
	result = FMOD::System_Create(&m_System);
	if (result != FMOD_OK)
	{
		printf("Failed to craete FMOD System !\n");
		return;
	}

	result = m_System->init(1, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		printf("Failed to initialize system !");
		//Cleanup
		result = m_System->close();
		if (result != FMOD_OK)
		{
			printf("Failed to close FMOD System !\n");
		}

		return;
	}

	printf("AudioManager::Initialize(): Successful !\n");

	//m_ChannelList.push_back(new Channel());
	//m_ChannelList[0]->volume = 1.f;
	//m_ChannelList[0]->pitch = 1.f;
	
	for (int i = 0; i < 10; i++)
	{
		m_ChannelList.push_back(new Channel);
	}

	m_Initialized = true;
}

void SoundManager::Destroy()
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;

	//Move this to a functiona, clear and release Audios from Audio Map
	for (std::pair<const char*, Audio*>pair : m_AudioMap)
	{
		result = pair.second->Audio->release();
		FMODCheckError(result);
	}

	m_AudioMap.clear();
	//Cleanup

	result = m_System->close();
	FMODCheckError(result);

	result = m_System->release();
	FMODCheckError(result);

	m_Initialized = false;
}

//Loads audio resource data when needed
void SoundManager::LoadSoundStream(const char* source)
{
	if (!m_Initialized)
		return;

	if (m_AudioMap.find(source) != m_AudioMap.end())
	{
		printf("AudioManager::LoadAudio() Audio already loaded!\n");
		return;
	}

	m_AudioMap.insert(std::pair<const char*, Audio*>(source, new Audio()));

	FMOD_RESULT result;
	result = m_System->createStream(source, FMOD_DEFAULT, 0, &m_AudioMap[source]->Audio);
	//m_Sound = m_AudioMap[source]->Audio;
	if (result != FMOD_OK)
	{
		printf("Failed to load the sound file !");
	}

	printf("SoundManager::LoadSound(%s): Loaded successful !\n", source );
}

//Loads media file directly into memory
void SoundManager::LoadSound(const char* file)
{
	if (!m_Initialized)
		return;

	if (m_AudioMap.find(file) != m_AudioMap.end())
	{
		printf("AudioManager::LoadAudio() Audio already loaded!\n");
		return;
	}

	m_AudioMap.insert(std::pair<const char*, Audio*>(file, new Audio()));

	FMOD_RESULT result;
	result = m_System->createSound(file, FMOD_DEFAULT, 0, &m_AudioMap[file]->Audio);
	//m_Sound = m_AudioMap[file]->Audio;
	if (result != FMOD_OK)
	{
		printf("Failed to load the sound file !");
	}

	printf("SoundManager::LoadSound(%s): Loaded successful !\n", file);
}

void SoundManager::InitializeChannelAttributes(int id)
{
	m_ChannelList[id]->looping = false;
	m_ChannelList[id]->paused = false;
	m_ChannelList[id]->pitch = 1.f;
	m_ChannelList[id]->volume = 1.f;
	m_ChannelList[id]->pan = 0;
}


int SoundManager::PlaySound(const char* SoundName)
{
	//m_SoundPaused = false;
	if (!m_Initialized)
	{
		printf("Not Initialized !\n");
		return m_NextChannelId;
	}

	std::map<const char*, Audio*>::iterator it = m_AudioMap.find(SoundName);
	if (it == m_AudioMap.end())
	{
		printf("Audio not found !\n");
		return m_NextChannelId;
	}

	FMOD_RESULT result;
	int channelId = m_NextChannelId;
	m_NextChannelId = (m_NextChannelId + 1) % 10;
	Channel* channel = m_ChannelList[channelId];

	m_ChannelList[channelId]->name = SoundName;

	InitializeChannelAttributes(channelId);

	result = m_System->playSound(it->second->Audio, 0, false, &channel->fmodChannel);
	FMODCheckError(result);

	return channelId;
	//if (result != FMOD_OK)
	//{
	//	printf("Failed to play sound !");
	//	//Cleanup
	//	result = m_Sound->release();
	//	FMODCheckError(result);

	//	result = m_System->close();	
	//	FMODCheckError(result);

	//	result = m_System->release();
	//	FMODCheckError(result);

	//	return;
	//}
}

bool SoundManager::IsChannelPlaying(int id)
{
	bool isPLaying;
	FMOD_RESULT result = m_ChannelList[id]->fmodChannel->isPlaying(&isPLaying);
	return isPLaying;
}

void SoundManager::GetPlaybackPosition(int id, unsigned int& value)
{
	FMOD_RESULT result = m_ChannelList[id]->fmodChannel->getPosition(&value, FMOD_TIMEUNIT_MS);
	FMODCheckError(result);
}

void SoundManager::GetAudioLength(const char* file, unsigned int& value)
{
	FMOD_RESULT result = m_AudioMap[file]->Audio->getLength(&value, FMOD_TIMEUNIT_MS);
	FMODCheckError(result);
}

void SoundManager::Update()
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;
	result = m_System->update();
	if (result != FMOD_OK)
	{
		FMODCheckError(result);
		Destroy();
	}
}

void SoundManager::SetChannelVolume(int id, float value)
{
	FMOD_RESULT result;
	m_ChannelList[id]->volume += value;

	if (m_ChannelList[id]->volume < 0)
	{
		m_ChannelList[id]->volume = 0;
	}
	result = m_ChannelList[id]->fmodChannel->setVolume(m_ChannelList[id]->volume);
	FMODCheckError(result);
}

void SoundManager::SetChannelPitch(int id, float value)
{
	FMOD_RESULT result;
	m_ChannelList[id]->pitch += value;

	if (m_ChannelList[id]->pitch < 0.2f)
	{
		m_ChannelList[id]->pitch = 0.2f;
	}

	result = m_ChannelList[id]->fmodChannel->setPitch(m_ChannelList[id]->pitch);
	FMODCheckError(result);
}

void SoundManager::SetChannelPan(int id, float value)
{
	FMOD_RESULT result;
	m_ChannelList[id]->pan += value;
	result = m_ChannelList[id]->fmodChannel->setPan(m_ChannelList[id]->pan);
	FMODCheckError(result);
}

float SoundManager::GetChannelVolume(int id)
{
	return m_ChannelList[id]->volume;
}

float SoundManager::GetChannelPitch(int id)
{
	return m_ChannelList[id]->pitch;
}

float SoundManager::GetChannelPan(int id)
{
	return m_ChannelList[id]->pan;
}
//
//void SoundManager::SetAudioName(int id)
//{
//	FMOD_RESULT result;
//	result = m_Sound->getName(m_ChannelList[id]->name, 150);
//	FMODCheckError(result);
//}

const char* SoundManager::GetAudioName(int id)
{
	return m_ChannelList[id]->name;
}

void SoundManager::PauseSound(int id)
{
	FMOD_RESULT result;

	if (!m_ChannelList[id]->paused)
	{
		m_ChannelList[id]->paused = true;
		result = m_ChannelList[id]->fmodChannel->setPaused(m_ChannelList[id]->paused);
		FMODCheckError(result);
	}
	else
	{
		m_ChannelList[id]->paused = false;
		result = m_ChannelList[id]->fmodChannel->setPaused(m_ChannelList[id]->paused);
		FMODCheckError(result);
	}
}

bool SoundManager::GetPauseStatus(int id)
{
	return m_ChannelList[id]->paused;
}

void SoundManager::StopAudio(int id)
{
	FMOD_RESULT result;

	result = m_ChannelList[id]->fmodChannel->stop();
	FMODCheckError(result);

	/*if (m_ChannelList[id]->paused)
		m_ChannelList[id]->paused = false;*/
}

void SoundManager::LoopAudio(int id)
{
	FMOD_RESULT result;

	if (m_ChannelList[id]->looping == false)
	{
		/*result = m_AudioMap[m_ChannelList[id]->name]->Audio->setMode(FMOD_LOOP_NORMAL);
		result = m_AudioMap[m_ChannelList[id]->name]->Audio->setLoopCount(3);*/
		result = m_ChannelList[id]->fmodChannel->setMode(FMOD_LOOP_NORMAL);
		result = m_ChannelList[id]->fmodChannel->setLoopCount(-1);
		m_ChannelList[id]->looping = true;
	}
	else
	{
		/*result = m_AudioMap[m_ChannelList[id]->name]->Audio->setMode(FMOD_DEFAULT);
		result = m_AudioMap[m_ChannelList[id]->name]->Audio->setLoopCount(0);*/
		result = m_ChannelList[id]->fmodChannel->setMode(FMOD_DEFAULT);
		result = m_ChannelList[id]->fmodChannel->setLoopCount(0);
		m_ChannelList[id]->looping = false;
	}
	FMODCheckError(result);
}

bool  SoundManager::GetLoopStatus(int id)
{
	return m_ChannelList[id]->looping;
}


void SoundManager::PrintInfo() const
{
	size_t size = m_ChannelList.size();

	for (size_t i = 0; i < m_ChannelList.size(); i++)
	{
	}
}
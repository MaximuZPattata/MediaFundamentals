#pragma once
#include <string>
#include "SoundManager.h"


struct sPlaybackPosition
{
	unsigned int position = 0;
	bool flag = false;
	unsigned int audio_length = 0;
	int bar_wait_time = 0;
	int temp = 0;
	int total_spaces = 50;
	int remaining_spaces = 0;
	int seconds_taken = 0;
	int minutes_taken = 0;
	int milliseconds_taken = 0;
	void InitializePlaybackVariables(sPlaybackPosition* playbackObj);
	void CalculatePlaybackPos(sPlaybackPosition* playbackObj);
	void PrintPlaybackPos(sPlaybackPosition* playbackObj);
};

struct sUserInterface
{
	const char* name;
	int volume;
	int pitch;
	int pan;
	int audio_id = 0;

	bool autoplay = false;
	bool playback_position_printed = false;
	bool error_response_displayed = false;

	const char* play_status;
	const char* loop_status;
	const char* autoplay_status = "OFF";

	void PrintInfo(const char* type);
	void ErasePreviousLines(int lineEraseCount);
	void CheckForPreviousLines();
	void PrintAudioDetails();
};

class cMediaPlayer
{
public:
	void InitiateMediaPlayer();
	cMediaPlayer() = default;
	~cMediaPlayer() {}

	void PrepareMediaPlayer();
	void DestroySoundManagerObj();
	void ReadFromFile();  
	void InitializeAudioData();

	void RunMediaPlayer();

private:
	void StopMediaPlayer();

	//inline void PrintInfo() const;

	int m_AudioChannel = 0;
	SoundManager* m_SoundManager;
	bool m_MediaPlayerRunning;
	const char* m_AudioNames[8];
	std::string m_AudioNamesFromFile[8];
};


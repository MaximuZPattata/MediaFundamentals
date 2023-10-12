#include "cMediaPlayer.h"
#include "AudioDemo.h"
#include <conio.h>
#include <string>
#include <iostream>
#include <fstream>

sUserInterface userInterface;
sPlaybackPosition playback_pos;

void cMediaPlayer::InitiateMediaPlayer()
{
	PrepareMediaPlayer();
	RunMediaPlayer();
	DestroySoundManagerObj();
}

void cMediaPlayer::InitializeAudioData()
{
	userInterface.CheckForPreviousLines();

	userInterface.play_status = "PLAYING";
	userInterface.loop_status = "OFF";

	m_AudioChannel = m_SoundManager->PlaySound(m_AudioNames[userInterface.audio_id]);
	userInterface.name = m_SoundManager->GetAudioName(m_AudioChannel);

	m_SoundManager->GetAudioLength(m_AudioNames[userInterface.audio_id], playback_pos.audio_length);

	playback_pos.InitializePlaybackVariables(&playback_pos);
}

void cMediaPlayer::PrepareMediaPlayer()
{
	m_SoundManager = new SoundManager();
	m_SoundManager->Initialize();
	ReadFromFile();
	for (int loop_count = 0; loop_count < 8; loop_count++)
	{
		m_SoundManager->LoadSound(m_AudioNames[loop_count]);
	}
	userInterface.PrintInfo("Menu");
}

void cMediaPlayer::ReadFromFile()
{
	std::ifstream AudioTextFile("AudioFiles.txt");
	if (AudioTextFile.is_open())
	{
		int line_count = 0;
		while (std::getline(AudioTextFile, m_AudioNamesFromFile[line_count]))
		{
			m_AudioNames[line_count] = m_AudioNamesFromFile[line_count].c_str();
			line_count++;
		}
		AudioTextFile.close();
	}
	else
		printf("Unable to open 'AudioFiles.txt'");
}

void sPlaybackPosition::InitializePlaybackVariables(sPlaybackPosition* playback_pos)
{
	playback_pos->bar_wait_time = playback_pos->audio_length / playback_pos->total_spaces;
	playback_pos->seconds_taken = 0;
	playback_pos->minutes_taken = 0;
	userInterface.playback_position_printed = true;
	userInterface.error_response_displayed = false;
	userInterface.volume = 50;
	userInterface.pitch = 0;
	userInterface.pan = 50;

	userInterface.PrintAudioDetails();
}

void sPlaybackPosition::CalculatePlaybackPos(sPlaybackPosition* playbackObj)
{
	//Formula for playback position------------------

	if (playbackObj->temp != (playbackObj->position / playbackObj->bar_wait_time) && playbackObj->flag == false)
	{
		playbackObj->temp = (playbackObj->position / playbackObj->bar_wait_time);

		if (playbackObj->temp > 50)
			temp = 50;

		playbackObj->remaining_spaces = abs(playbackObj->total_spaces - playbackObj->temp);

		//Calculating duration---------------
		playbackObj->seconds_taken = int(playbackObj->position / 1000);

		if (playbackObj->seconds_taken >= 60)
		{
			playbackObj->minutes_taken = playbackObj->seconds_taken / 60;
			playbackObj->seconds_taken = abs(playbackObj->minutes_taken * 60 - playbackObj->seconds_taken);
		}

		playbackObj->milliseconds_taken = position % 1000;
	
		//Calculation ends-------------------

		playbackObj->flag = true;
		
		PrintPlaybackPos(&playback_pos);

	}
	else if (playbackObj->flag == true && playbackObj->temp != (playbackObj->position / playbackObj->bar_wait_time))
	{
		playbackObj->flag = false;
	}

	//Formula ends--------------------------------
}

void sPlaybackPosition::PrintPlaybackPos(sPlaybackPosition* playbackObj)
{
	printf("\r[ ");

	for (int i = 0; i < playbackObj->temp; i++)
	{
			printf(" ");
	}

	printf("|");

	for (int i = 0; i < playbackObj->remaining_spaces; i++)
	{
		printf(" ");
	}

	printf(" ]  DURATION = %d : %d : %d", playbackObj->minutes_taken, playbackObj->seconds_taken, playbackObj->milliseconds_taken);
}

void cMediaPlayer::RunMediaPlayer()
{
	m_MediaPlayerRunning = true;

	while (m_MediaPlayerRunning)
	{
		m_SoundManager->Update();

		if (_kbhit())
		{
			int key = _getch();
			if (key == '1')
			{
				userInterface.audio_id = 0;	
				InitializeAudioData();
			}
			else if (key == '2')
			{
				userInterface.audio_id = 1;
				InitializeAudioData();
			}
			else if (key == '3')
			{
				userInterface.audio_id = 2;
				InitializeAudioData();
			}
			else if (key == '4')
			{
				userInterface.audio_id = 3;
				InitializeAudioData();
			}
			else if (key == '5')
			{
				userInterface.audio_id = 4;
				InitializeAudioData();
			}
			else if (key == '6')
			{
				userInterface.audio_id = 5;
				InitializeAudioData();
			}
			else if (key == '7')
			{
				userInterface.audio_id = 6;
				InitializeAudioData();
			}
			else if (key == '8')
			{
				userInterface.audio_id = 7;
				InitializeAudioData();
			}
			else if (key == 80/*Indicating 'P'*/ || key == 112/*Indicating 'p'*/)
			{
				if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
				{
					m_SoundManager->PauseSound(m_AudioChannel);

					if (m_SoundManager->GetPauseStatus(m_AudioChannel))
					{
						userInterface.ErasePreviousLines(2);
						userInterface.play_status = "PAUSED";
						userInterface.PrintInfo("Status");

						playback_pos.PrintPlaybackPos(&playback_pos);
					}
					else
					{
						userInterface.ErasePreviousLines(2);
						userInterface.play_status = "PLAYING";
						userInterface.PrintInfo("Status");
					}
				}
				else
				{
					userInterface.PrintInfo("Error");
				}
			}
			else if (key == 83/*Indicating 'S'*/ || key == 115/*Indicating 's'*/)
			{
				if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
				{
					m_SoundManager->StopAudio(m_AudioChannel);

					playback_pos.flag = false;
					playback_pos.position = 0;

					if (userInterface.autoplay)
					{
						userInterface.autoplay = false;
						userInterface.autoplay_status = "OFF";
					}

					userInterface.ErasePreviousLines(2);
					userInterface.loop_status = "OFF";
					userInterface.play_status = "STOPPED";
					userInterface.PrintInfo("Status");

					playback_pos.CalculatePlaybackPos(&playback_pos);
				}
				else
				{
					userInterface.PrintInfo("Error");
				}
			}
			else if (key == 76/*Indicating 'L'*/ || key == 108/*Indicating 'l'*/)
			{
				if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
				{
					m_SoundManager->LoopAudio(m_AudioChannel);

					if (userInterface.autoplay)
					{
						userInterface.autoplay = false;
						userInterface.autoplay_status = "OFF";
					}

					if (m_SoundManager->GetLoopStatus(m_AudioChannel))
					{
						userInterface.ErasePreviousLines(2);
						userInterface.loop_status = "ON";
						userInterface.PrintInfo("Status");
					}
					else
					{
						userInterface.ErasePreviousLines(2);
						userInterface.loop_status = "OFF";
						userInterface.PrintInfo("Status");
					}

					if (m_SoundManager->GetPauseStatus(m_AudioChannel))
					{
						playback_pos.PrintPlaybackPos(&playback_pos);
					}
				}
				else
				{
					userInterface.PrintInfo("Error");
				}
			}
			else if (key == 65/*Indicating 'A'*/ || key == 97/*Indicating 'a'*/)
			{
					if (userInterface.autoplay)
					{
						userInterface.autoplay = false;
						userInterface.autoplay_status = "OFF";
						userInterface.ErasePreviousLines(2);
						userInterface.PrintInfo("Status");
					}
					else
					{
						userInterface.autoplay = true;
						userInterface.autoplay_status = "ON";

						if (userInterface.play_status == "STOPPED" || !(m_SoundManager->IsChannelPlaying(m_AudioChannel)))
						{
							if (userInterface.audio_id > 9)
								userInterface.audio_id = 0;

							InitializeAudioData();
						}
						else
						{
							userInterface.ErasePreviousLines(2);
							userInterface.PrintInfo("Status");
						}

						if (m_SoundManager->GetLoopStatus(m_AudioChannel))
						{
							m_SoundManager->LoopAudio(m_AudioChannel);
							userInterface.ErasePreviousLines(2);
							userInterface.loop_status = "OFF";
							userInterface.PrintInfo("Status");
						}
					}

					if (m_SoundManager->GetPauseStatus(m_AudioChannel))
					{
						playback_pos.PrintPlaybackPos(&playback_pos);
					}
			}

			else if (key == 88/*Indicating 'X'*/ || key == 120/*Indicating 'x'*/)
			{
				if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
				{
					if (m_SoundManager->GetChannelVolume(m_AudioChannel) < 2.f)
					{
						m_SoundManager->SetChannelVolume(m_AudioChannel, 0.2f);
						userInterface.volume = (m_SoundManager->GetChannelVolume(m_AudioChannel) * 100) / 2;
						userInterface.ErasePreviousLines(6);
						userInterface.PrintInfo("Attributes");
						userInterface.PrintInfo("Status");

						if (m_SoundManager->GetPauseStatus(m_AudioChannel))
						{
							playback_pos.PrintPlaybackPos(&playback_pos);
						}
					}
				}
				else
				{
					userInterface.PrintInfo("Error");
				}
			}

			else if (key == 90/*Indicating 'Z'*/ || key == 122/*Indicating 'z'*/)
			{
				if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
				{
					if (m_SoundManager->GetChannelVolume(m_AudioChannel) > 0)
					{
						m_SoundManager->SetChannelVolume(m_AudioChannel, -0.2f);
						userInterface.volume = (m_SoundManager->GetChannelVolume(m_AudioChannel) * 100) / 2;
						userInterface.ErasePreviousLines(6);
						userInterface.PrintInfo("Attributes");
						userInterface.PrintInfo("Status");

						if (m_SoundManager->GetPauseStatus(m_AudioChannel))
						{
							playback_pos.PrintPlaybackPos(&playback_pos);
						}
					}
				}
				else
				{
					userInterface.PrintInfo("Error");
				}
			}

			else if (key == 86/*Indicating 'V'*/ || key == 118/*Indicating 'v'*/)
			{
				if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
				{
					if (m_SoundManager->GetChannelPitch(m_AudioChannel) < 1.8f)
					{
						m_SoundManager->SetChannelPitch(m_AudioChannel, 0.2f);
						userInterface.pitch +=25;
						userInterface.ErasePreviousLines(6);
						userInterface.PrintInfo("Attributes");
						userInterface.PrintInfo("Status");

						if (m_SoundManager->GetPauseStatus(m_AudioChannel))
						{
							playback_pos.PrintPlaybackPos(&playback_pos);
						}
					}
					
				}
				else
				{
					userInterface.PrintInfo("Error");
				}
			}

			else if (key == 67/*Indicating 'C'*/ || key == 99/*Indicating 'c'*/)
			{
				if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
				{
					if (m_SoundManager->GetChannelPitch(m_AudioChannel) > 0.4f)
					{
						m_SoundManager->SetChannelPitch(m_AudioChannel, -0.2f);
						userInterface.pitch -= 25;
						userInterface.ErasePreviousLines(6);
						userInterface.PrintInfo("Attributes");
						userInterface.PrintInfo("Status");
						playback_pos.PrintPlaybackPos(&playback_pos);
					}
				}
				else
				{
					userInterface.PrintInfo("Error");
				}
			}

			else if (key == 78/*Indicating 'N'*/ || key == 110/*Indicating 'n'*/)
			{
				if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
				{
					if (m_SoundManager->GetChannelPan(m_AudioChannel) < 0.9f)
					{
						m_SoundManager->SetChannelPan(m_AudioChannel, 0.2f);
						userInterface.pan += 10;
						userInterface.ErasePreviousLines(6);
						userInterface.PrintInfo("Attributes");
						userInterface.PrintInfo("Status");

						if (m_SoundManager->GetPauseStatus(m_AudioChannel))
						{
							playback_pos.PrintPlaybackPos(&playback_pos);
						}
					}
				}
				else
				{
					userInterface.PrintInfo("Error");
				}
			}

			else if (key == 66/*Indicating 'B'*/ || key == 98/*Indicating 'b'*/)
			{
				if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
				{
					if (m_SoundManager->GetChannelPan(m_AudioChannel) > -0.9f)
					{
						m_SoundManager->SetChannelPan(m_AudioChannel, -0.2f);
						userInterface.pan -= 10;
						userInterface.ErasePreviousLines(6);
						userInterface.PrintInfo("Attributes");
						userInterface.PrintInfo("Status");
						playback_pos.PrintPlaybackPos(&playback_pos);
					}
				}
				else
				{
					userInterface.PrintInfo("Error");
				}
			}

			if (key == 27/*ESC*/)
			{
				StopMediaPlayer();
			}
		}
		if (m_SoundManager->IsChannelPlaying(m_AudioChannel))
		{
			m_SoundManager->GetPlaybackPosition(m_AudioChannel, playback_pos.position);

			playback_pos.CalculatePlaybackPos(&playback_pos);
		}
		else if(userInterface.autoplay)
		{
			userInterface.audio_id++;
			
			if(userInterface.audio_id > 9)
				userInterface.audio_id = 0;

			InitializeAudioData();
		}
	}
}

void cMediaPlayer::DestroySoundManagerObj()
{
	m_SoundManager->Destroy();
	delete m_SoundManager;
}

void cMediaPlayer::StopMediaPlayer()
{
	m_MediaPlayerRunning = false;
}


void sUserInterface::PrintInfo(const char* type)
{
	if (type == "Menu")
	{
		printf("------------------------------------------------------------------------------------------");
		printf("\n\n<<MEDIA PLAYER IS READY TO USE>> \nPress any of the following keys to play and manipulate audio : \n\n 1. Press any number from '0 - 9' to play the audio \n");
		printf(" 2. Press 'P' to PAUSE and RESUME the current audio \n 3. Press 'S' to STOP the audio \n 4. Press 'L' to LOOP the same audio and press again to stop loop \n");
		printf(" 5. Press 'X' to increase the VOLUME of the current audio and 'Z' to decrease it \n 6. Press 'V' to increase the PITCH of the current audio and 'C' to decrease it \n");
		printf(" 7. Press 'N' to increase the PAN of the current audio and 'B' to decrease it \n 8. Press 'A' to turn on AUTOPLAY. Press again to turn it off.");
		// 3. Press 'S' to stop the audio \n 4. Press 'L' to loop the same audio and press again to stop loop \n ");
	}

	else if (type == "Error")
	{
		userInterface.CheckForPreviousLines();
		printf("ERROR : There is no audio playing currently. Press any number key from '0' - '9' to play an audio.");
		userInterface.error_response_displayed = true;
	}

	else if (type == "Attributes")
	{
		printf("---> VOLUME = %d%% \n", volume);
		printf("---> PITCH  = %d%% \n", pitch);
		printf("---> PAN    = %d%% \n\n", pan);
	}

	else if (type == "Status")
	{
		printf("STATUS : %s  ||  LOOP : %s  ||  AUTOPLAY : %s\n\n", play_status, loop_status, autoplay_status);
	}

}

void sUserInterface::PrintAudioDetails()
{
	printf("Audio Name : %s\n", userInterface.name);
	printf("Other details : *Details of Audio*\n");
	userInterface.PrintInfo("Attributes");
	userInterface.PrintInfo("Status");
}

void sUserInterface::ErasePreviousLines(int lineEraseCount)
{
	for(int i=0; i<lineEraseCount; i++)
		printf("\r\033[K\033[1A");
	
	printf("\r\033[K");
}

void sUserInterface::CheckForPreviousLines()
{
	if (userInterface.playback_position_printed == false && userInterface.error_response_displayed == false)
		printf("\n\n");
	else if (userInterface.playback_position_printed == true && userInterface.error_response_displayed == false)
	{
		userInterface.ErasePreviousLines(8);
		userInterface.playback_position_printed = false;
	}
	else if (userInterface.playback_position_printed == false && userInterface.error_response_displayed == true)
	{
		userInterface.ErasePreviousLines(0);
		userInterface.error_response_displayed = false;
	}
}



//AutoPlayNextSong
//ReadMe
//Documentation and code cleanup
//UserInterface separate headerfile
//Add new audio
//Print other audio details 
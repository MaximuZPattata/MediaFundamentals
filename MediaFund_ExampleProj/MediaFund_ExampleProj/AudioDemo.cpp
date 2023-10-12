#include "AudioDemo.h"
#include <conio.h>

//void AudioDemo::Prepare()
//{
//	m_SoundManager = new SoundManager();
//	m_SoundManager->Initialize();
//	m_SoundManager->LoadSound("Audio/jaguar.wav");
//	m_SoundManager->LoadSound("Audio/swish.wav");
//	m_SoundManager->LoadSound("Audio/singing.wav");
//}
//
//void AudioDemo::Shutdown()
//{
//	m_SoundManager->Destroy();
//	delete m_SoundManager;
//}
//
//void AudioDemo::RunDemo()
//{
//	bool m_Running = true;
//	int channel = 0;
//	unsigned int position = 0;
//	bool flag = false;
//	unsigned int audio_length = 0;
//	int bar_wait_time = 0;
//	int temp = 0;
//	int total_spaces = 50;
//	int remaining_spaces = 0;
//	int seconds_taken = 0;
//	int minutes_taken = 0;
//
//	while (m_Running)
//	{
//		m_SoundManager->Update();
//		//PrintInfo();
//		if (_kbhit())
//		{
//			int key = _getch();
//			if (key == '1')
//			{
//				printf("\n");
//				channel = m_SoundManager->PlaySound("Audio/jaguar.wav");
//				m_SoundManager->GetAudioLength("Audio/jaguar.wav", audio_length);
//				m_SoundManager->SetChannelPitch(channel, 2.f);
//				bar_wait_time = audio_length / total_spaces;
//				seconds_taken = 0;
//				minutes_taken = 0;
//			}
//			else if (key == '2')
//			{
//				printf("\n");
//				channel = m_SoundManager->PlaySound("Audio/swish.wav");
//				m_SoundManager->GetAudioLength("Audio/swish.wav", audio_length);
//				m_SoundManager->SetChannelPitch(channel, 1.f);
//				bar_wait_time = audio_length / total_spaces;
//				seconds_taken = 0;
//				minutes_taken = 0;
//			}
//			else if (key == '3')
//			{
//				printf("\n");
//				channel = m_SoundManager->PlaySound("Audio/singing.wav");
//				m_SoundManager->GetAudioLength("Audio/singing.wav", audio_length);
//				m_SoundManager->SetChannelPitch(channel, 2.f);
//				bar_wait_time = audio_length / total_spaces;
//				seconds_taken = 0;
//				minutes_taken = 0;
//			}
//			if (key == 27/*ESC*/)
//			{
//				StopDemo();
//			}
//		}
//		if (m_SoundManager->IsChannelPlaying(channel))
//		{
//			m_SoundManager->GetPlaybackPosition(channel, position);
//			
//			//Formula for playback position------------------
//
//			if (temp != (position / bar_wait_time) && flag == false)
//			{
//				temp = (position / bar_wait_time);
//
//				printf("\r[ ");
//
//				for (int i = 0; i < (position / bar_wait_time); i++)
//				{
//					printf(" ");
//				}
//
//				printf("|");
//
//				remaining_spaces = total_spaces - int(position / bar_wait_time);
//
//				for (int i = 0; i < abs(remaining_spaces); i++)
//				{
//					printf(" ");
//				}
//
//				//Calculating duration---------------
//				seconds_taken = int(position / 60);
//
//				if (seconds_taken >= 60)
//				{
//					minutes_taken = seconds_taken / 60;
//					seconds_taken = abs(minutes_taken * 60 - seconds_taken);
//				}
//				//Calculation ends-------------------
//			
//				printf(" ]  DURATION = %d : %d ", minutes_taken, seconds_taken);
//				
//				flag = true;
//			}
//			else if (flag == true && temp != (position / bar_wait_time))
//			{
//				flag = false;
//			}
//
//			//Formula ends--------------------------------
//		}
//	}
//}
//
//void AudioDemo::PrintInfo() const
//{
//	m_SoundManager->PrintInfo();
//}
//
//void AudioDemo::StopDemo()
//{
//	m_Running = false;
//}
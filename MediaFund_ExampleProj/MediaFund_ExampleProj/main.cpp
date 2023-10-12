#include "SoundManager.h" 
#include "AudioDemo.h"
#include "cMediaPlayer.h"
#include<iostream>
#include<conio.h>

//SoundManager g_SoundManager;

//void PlaySound(const char* soundfile)
//{
//	g_SoundManager.LoadSound(soundfile);
//	g_SoundManager.PlaySound();
//
//	printf("Sound playing, press ESC to quit...");
//	while (true)
//	{
//		g_SoundManager.Update();
//
//		//std::cin >> //This is a blocking call
//
//		//conio.h _kbhit() _getch() //non-blocking solution
//		if (_kbhit())
//		{
//			int key = _getch();
//			if (key == 27/*ESC*/)
//			{
//				break;
//			}
//		}
//	}
//}

int main(int argc, char** argv)
{
	cMediaPlayer mediaPlay;
	mediaPlay.InitiateMediaPlayer();
}

/*g_SoundManager.Initialize();

	PlaySound("Audio/swish.wav");
	PlaySound("Audio/jaguar.wav");
	PlaySound("Audio/singing.wav");

	g_SoundManager.Destroy();*/

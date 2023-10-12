#pragma once

#include "SoundManager.h"

class AudioDemo
{
public:
	AudioDemo() = default;
	~AudioDemo() {}

	void Prepare();
	void Shutdown();

	void RunDemo();

private:
	void StopDemo();

	inline void PrintInfo() const;

	SoundManager* m_SoundManager;
	bool m_Running;
};


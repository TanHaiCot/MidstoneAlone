#pragma once

#ifndef TIMER_H
#define TIMER_H

/// <summary>
/// A class to handle time change when doing simulation
/// </summary>
class Timer {
	unsigned int prevTicks;
	unsigned int currTicks;

public:
	Timer();
	~Timer();

	void Start();
	void Stop();
	void Pause();
	void Unpause();

	int getTick(); 

	bool isStart(); 
	bool isPause(); 

	void UpdateFrameTicks();
	float GetDeltaTime() const;
	unsigned int GetSleepTime(const unsigned int fps) const;

private:
	int startTick; 
	int pausedTick;

	bool isStarted;
	bool isPaused; 
};

#endif
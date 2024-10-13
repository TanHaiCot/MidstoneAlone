#pragma once

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

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
	double GetElapsedMilliseconds();

	bool IsRunning(); 
	bool IsPause(); 

	void UpdateFrameTicks();
	float GetDeltaTime() const;
	unsigned int GetSleepTime(const unsigned int fps) const;

private:
	int startTick; 
	int pausedTick;

	std::chrono::high_resolution_clock::time_point startTime; 
	std::chrono::steady_clock::time_point pauseTime; 

	bool isRunning;
	bool isPaused; 
};

#endif
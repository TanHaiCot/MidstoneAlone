#include "CommonFunctions.h"
#include "Timer.h"



Timer::Timer() {
	prevTicks = 0;
	currTicks = 0;
	startTick = 0;
	pausedTick = 0;
	isStarted = false;
	isPaused = false;
}


Timer::~Timer() {

}


void Timer::Start() {
	isStarted = true;
	isPaused = false;
	startTick = SDL_GetTicks();
}


void Timer::Stop()
{
	isPaused = false;
	isStarted = false;
}


void Timer::Pause()
{
	if (isStarted == true && isPaused == false)
	{
		isPaused = true;
		pausedTick = SDL_GetTicks() - startTick; //current moment - passed time 
	}
}


void Timer::Unpause()
{
	if (isPaused = true)
	{
		isPaused = false;
		startTick = SDL_GetTicks() - pausedTick;
		pausedTick = 0;
	}
}


int Timer::getTick()
{
	if (isStarted == true)
	{
		if (isPaused == true)
		{
			return pausedTick;
		}
		else { return SDL_GetTicks() - startTick; }
	}
	return 0;
}


bool Timer::isStart()
{
	return isStarted;
}


bool Timer::isPause()
{
	return isPaused;
}


//float Timer::GetDeltaTime() const {
//	return (float(currTicks - prevTicks)) / 1000.0f;
//}
//
//unsigned int Timer::GetSleepTime(const unsigned int fps) const {
//	unsigned int milliSecsPerFrame = 1000 / fps;
//	if (milliSecsPerFrame == 0) {
//		return 0;
//	}
//
//	unsigned int sleepTime = milliSecsPerFrame - (SDL_GetTicks() - currTicks);
//	if (sleepTime > milliSecsPerFrame) {
//		return milliSecsPerFrame;
//	}
//
//	return sleepTime;
//}
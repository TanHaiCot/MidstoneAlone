#include "CommonFunctions.h"
#include "Timer.h"



Timer::Timer() {
	/*prevTicks = 0;
	currTicks = 0;
	startTick = 0;
	pausedTick = 0;
	isRunning = false;
	isPaused = false;*/
	startTime = std::chrono::high_resolution_clock::now();
}


Timer::~Timer() {

}


void Timer::Start() {
	startTime = std::chrono::high_resolution_clock::now(); 
	//isRunning = true;
	//isPaused = false;
	//startTick = SDL_GetTicks();

}


void Timer::Stop()
{
	isPaused = false; 
	isRunning = false;
}


void Timer::Pause()
{
	if (isRunning && !isPaused)
	{
		pauseTime = std::chrono::steady_clock::now(); 
		isPaused = true; 
		//pausedTick = SDL_GetTicks() - startTick; //current moment - passed time 
	}
}


void Timer::Unpause()
{
	if (isRunning && isPaused)
	{
		auto pauseDuration = std::chrono::steady_clock::now() - pauseTime; 
		startTime += pauseDuration; 
		isPaused = false; 
		//startTick = SDL_GetTicks() - pausedTick; 
		//pausedTick = 0; 
	}
}


//int Timer::getTick()
//{
//	if (isRunning == true)
//	{
//		if (isPaused == true)
//		{
//			return pausedTick; 
//		}
//		else { return SDL_GetTicks() - startTick;  }
//	}
//	return 0; 
//}

double Timer::GetElapsedMilliseconds()
{
	auto endTime = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double, std::milli>(endTime - startTime).count(); 
}


bool Timer::IsRunning()
{
	return isRunning;
}


bool Timer::IsPause()
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
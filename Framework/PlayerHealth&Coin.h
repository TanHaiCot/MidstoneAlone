#pragma once

#ifndef PlAYER_HEALTH_COIN_H
#define PLAYER_HEALTH_COIN_H

#include "CommonFunctions.h"
#include "BaseObjects.h"

class PlayerHealth : public BaseObjects
{
public:
	PlayerHealth();
	~PlayerHealth();

	void SetNumber(const int& num) { number = num; }
	void AddPos(const int& xPos); 
	void Show(SDL_Renderer* screen); 
	void Init(SDL_Renderer* screen); 

	void Increase();
	void Decrease();

private:
	int number; 
	std::vector<int> posList; 

};


class PlayerCoin : public BaseObjects
{
public:
	PlayerCoin();
	~PlayerCoin(); 

	void Init(SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	void SetPos(const int& xpos, const int& ypos) { xPOS = xpos, yPOS = ypos; }

private:
	int xPOS; 
	int yPOS; 
};

#endif 
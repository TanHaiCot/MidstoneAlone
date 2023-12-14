#pragma once

#ifndef PlAYER_HEALTH_COIN_H
#define PLAYER_HEALTH_COIN_H

#include "CommonFunctions.h"
#include "BaseObjects.h"

class PlayerHealthCoin : public BaseObjects
{
public:
	PlayerHealthCoin();
	~PlayerHealthCoin();

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

#endif 
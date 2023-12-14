#include "PlayerHealth&Coin.h"

PlayerHealthCoin::PlayerHealthCoin()
{
	number - 0;
}


PlayerHealthCoin::~PlayerHealthCoin()
{

}


void PlayerHealthCoin::AddPos(const int& xPos)
{
	posList.push_back(xPos);

}


void PlayerHealthCoin::Init(SDL_Renderer* screen)
{
	loadImage("textures/player_pw.png", screen);
	number = 3; 
	if (posList.size() > 0)
	{
		posList.clear();
	}

	AddPos(20);
	AddPos(60);
	AddPos(100);
}


void PlayerHealthCoin::Show(SDL_Renderer* screen)
{
	for (int i = 0; i < posList.size(); i++)
	{
		rect.x = posList.at(i);
		rect.y = 0;
		Render(screen); 
	}
}


void PlayerHealthCoin::Decrease()
{
	number--; 
	posList.pop_back(); 
}


void PlayerHealthCoin::Increase()
{
	number++; 
	int lastPos = posList.back();
	lastPos += 40; 
	posList.push_back(lastPos); 
}

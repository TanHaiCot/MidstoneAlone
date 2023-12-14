#pragma once

#ifndef TEXT_H
#define TEXT_H

#include "CommonFunctions.h"

class Text
{
public:
	Text();
	~Text();

	enum TextColor
	{
		RED_TEXT = 0,
		WHITE_TEXT = 1,
		BLACK_TEXT = 2, 
	};

	bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen); 
	void Free(); 

	void SetColor(Uint8 red, Uint8 green, Uint8 blue); 
	void SetColor(int type); 

	void RenderText(SDL_Renderer* screen, 
		            int xPos, int yPos, 
		            SDL_Rect* clip = nullptr,  
		            double angle = 0.0, 
		            SDL_Point* center = nullptr, 
		            SDL_RendererFlip flip = SDL_FLIP_NONE);
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	void SetText(const std::string& text) { stringValue = text; }
	std::string GetText() const { return stringValue; }

private:
	std::string stringValue;
	SDL_Color textColor; 
	SDL_Texture* texture; 
	int width;
	int height; 

};

#endif 
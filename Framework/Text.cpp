#include "Text.h"

Text::Text()
{
	textColor.r = 255;
	textColor.g = 255;
	textColor.b = 255;
	texture = nullptr; 
}


Text::~Text()
{
}


bool Text::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen) 
{
	SDL_Surface* TextSurface = TTF_RenderText_Solid(font, stringValue.c_str(), textColor); 
	if (TextSurface)
	{
		texture = SDL_CreateTextureFromSurface(screen, TextSurface);
		width = TextSurface->w;
		height = TextSurface->h; 

		SDL_FreeSurface(TextSurface); 
	}

	return texture != nullptr;
}


void Text::Free()
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr; 
	}
}


void Text::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	textColor.r = red;
	textColor.g = green;
	textColor.b = blue; 
}


void Text::SetColor(int type)
{
	if (type == RED_TEXT)
	{
		SDL_Color color = { 255, 0, 0 };
		textColor = color;
	}
	else if (type == WHITE_TEXT)
	{
		SDL_Color color = { 255, 255, 255 };
		textColor = color;
	}
	else if (type == BLACK_TEXT)
	{
		SDL_Color color = { 0, 0, 0 };
		textColor = color;
	}
}


void Text::RenderText(SDL_Renderer* screen,
	                  int xPos, int yPos, SDL_Rect* clip /*= nullptr*/,
	                  const double angle /*= 0.0*/, SDL_Point* center /*= nullptr*/,
	                  SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
	SDL_Rect RenderQuad = { xPos, yPos, width, height };
	if (clip != nullptr)
	{
		RenderQuad.w = clip->w;
		RenderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(screen, texture, &RenderQuad, nullptr, angle, center, flip); 
}
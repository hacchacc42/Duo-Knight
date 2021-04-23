#include "Text.h"
#include <iostream>
Text::Text( int font_size, std::string text, const SDL_Color& color, SDL_Renderer* renderer)
{
	_renderer = renderer;
	_font = TTF_OpenFont(_font_path.c_str(), font_size);
	if (!_font)
	{
		std::cout << "Failed to load font" << std::endl;
	}
	loadFont(text, color);
	SDL_QueryTexture(_text_texture, nullptr, nullptr, &_text_rect.w, &_text_rect.h);
}
Text::~Text()
{

}
void Text::Render(int x, int y) // Static Text
{
	_text_rect.x = x;
	_text_rect.y = y;
	SDL_RenderCopy(_renderer, _text_texture, nullptr, &_text_rect);

}


void Text::loadFont( std::string text, const SDL_Color& color)
{
	
	SDL_DestroyTexture(_text_texture);
	_text_surface = TTF_RenderText_Solid(_font, text.c_str(), color);
	_text_texture = SDL_CreateTextureFromSurface(_renderer,_text_surface);
	SDL_FreeSurface(_text_surface);
}

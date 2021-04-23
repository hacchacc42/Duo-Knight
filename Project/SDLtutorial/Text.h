#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Text
{
public:
	Text( int font_size, std::string text, const SDL_Color &color, SDL_Renderer* renderer);
	~Text();
	void Render(int x, int y);  // Static Text

	 void loadFont( std::string text, const SDL_Color& color);
private:
	SDL_Texture* _text_texture = nullptr;
	SDL_Rect _text_rect;
	SDL_Surface* _text_surface;
	SDL_Renderer* _renderer;
	TTF_Font* _font;
	const std::string _font_path = "Images/df.ttf";
};
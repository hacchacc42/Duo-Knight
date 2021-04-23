#include "Texture2D.h"
using namespace std;
Texture2D::Texture2D(SDL_Renderer* renderer)
{
	this->m_renderer = renderer;
}
Texture2D::~Texture2D()
{
	//Free up memory
	Free();

	m_renderer = nullptr;
}
void Texture2D::Free()
{
	//check if texture exists before removing it
	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
}
void Texture2D::Render(Vector2D new_position, SDL_RendererFlip flip, double angle)
{
	SDL_Rect renderLocation = { new_position.x,new_position.y,m_width, m_height };
	//Render to screen
	SDL_RenderCopyEx(m_renderer, m_texture, NULL, &renderLocation, 0, NULL, flip);
}
void Texture2D::Render(SDL_Rect src_rect, SDL_Rect src_dest, SDL_RendererFlip flip, double angle)
{
	SDL_RenderCopyEx(m_renderer, m_texture, &src_rect, &src_dest, angle, nullptr, flip);
}
bool Texture2D::LoadFromFile(std::string path)
{

	Free();
	//Load the image
	SDL_Surface* p_surface = IMG_Load(path.c_str());
	if (p_surface != nullptr)
	{
		//create the texture from the pixels on the surface
		m_texture = SDL_CreateTextureFromSurface(m_renderer, p_surface);
		if (m_texture == nullptr)
		{
			//colour key the image to be transparent
			SDL_SetColorKey(p_surface, SDL_TRUE, SDL_MapRGB(p_surface->format, 0, 0XFF, 0XFF));
		}
		//remove the loaded surface now that we have a texture
		m_width = p_surface->w;
		m_height = p_surface->h;
		SDL_FreeSurface(p_surface);

	}
	else
	{
		cout << "Unable to create texture from surface. Error: " << IMG_GetError();
	}
	//Return the texture
	return m_texture != nullptr;
}
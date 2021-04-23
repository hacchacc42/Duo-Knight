#pragma once
#ifndef MACRO

#include <SDL.h>
#include "Coin.h"
#include "Commons.h"
#include "Character.h"
#include "C_Player_1.h"
#include "C_Player_2.h"
#include "C_Mimic.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include "StaticBlock.h"
#include "Texture2D.h"
#include "Collision.h"
#include "Text.h"
#include "UI.h"
#include <vector>
class GameScreen
{
public:
	GameScreen(SDL_Renderer* renderer);
	virtual ~GameScreen();
	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
protected:
	SDL_Renderer* m_renderer;
};
#endif // !MACRO
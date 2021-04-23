#pragma once
#include "GameScreen.h"

class GameScreenLose : GameScreen
{
public:
	GameScreenLose(SDL_Renderer* renderer, int FP_S, int FP_L, int SP_S, int SP_L);
	~GameScreenLose();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	void CreateUI();

	bool _quit;
private:

	std::vector<UI*> _UI;

	std::vector<Texture2D*> m_background_texture;
	Mix_Chunk* soundEffect;
	bool SetUpLevel();

	LevelMap* m_level_map;
};



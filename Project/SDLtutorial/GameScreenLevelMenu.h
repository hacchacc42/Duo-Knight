#pragma once
#include "GameScreen.h"

//IN CASE IT BREAKE, THERE WAS SOME FORWARD DECLARATION FOR TEXTURE

class GameScreenLevelMenu : GameScreen
{
public:
	GameScreenLevelMenu(SDL_Renderer* renderer);
	~GameScreenLevelMenu();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	void CreateUI();
	void CreateText();
	void LoadFile();
	void SaveFile();

	bool _quit;
private:

	std::vector<UI*> _UI;

	std::string FileName[10];
	int FileScore[10];

	std::vector<Texture2D*> m_background_texture;
	Mix_Chunk* soundEffect;
	bool SetUpLevel();

	LevelMap* m_level_map;

	float _offset;
	int _menu;
	int _select;
	std::vector<Text*> _HST;

	bool quitSelected = false;
};


#pragma once
#include "GameScreen.h"

class GameScreenWin : GameScreen
{
public:
	GameScreenWin(SDL_Renderer* renderer, int FP_S, int FP_L, int SP_S, int SP_L);
	~GameScreenWin();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	void CreateUI();
	void CreateText();

	void ChangeLetter(int pos);
	void LoadFile();
	void SaveFile();
	int Sorting(int currentScore, std::string currentName);
private:

	int _FP_L;
	int _FP_S;
	int _SP_L;
	int _SP_S;

	int letPos;

	int _score;
	std::string _name;

	std::string Name[3];
	
	std::string FileName[10];
	int FileScore[10];

	std::vector<UI*> _UI;

	std::vector<Texture2D*> m_background_texture;
	Mix_Chunk* soundEffect;
	bool SetUpLevel();

	LevelMap* m_level_map;
	Vector2D _select;
	std::vector<Text*> _HST;
};


#pragma once
#include "GameScreen.h"

//IN CASE IT BREAKE, THERE WAS SOME FORWARD DECLARATION FOR TEXTURE

class GameScreenLevel1 : GameScreen
{
public:
	GameScreenLevel1(SDL_Renderer* renderer, int SP_S,int SP_L);
	~GameScreenLevel1();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	void SetLevelMap();
	void TileCollision();
	void CreateUI();
	void CreateText();
	void PlayerCollision(Character *player);

private:

	int _SP_L;
	int _SP_S;

	std::vector<StaticBlock*> m_static_block;
	std::vector<Coin*> _coin;

	std::vector<UI*> _UI;

	Mix_Chunk* _s_coinCollect;

	Vector2D my_char2_pos;
	Character* my_character2;

	Texture2D* m_background_texture;

	Text* _text;
	Text* _text2;

	bool SetUpLevel();

	LevelMap* m_level_map;

};


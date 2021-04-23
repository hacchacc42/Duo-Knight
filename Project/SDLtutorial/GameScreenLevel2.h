#pragma once
#include "GameScreen.h"

//IN CASE IT BREAKE, THERE WAS SOME FORWARD DECLARATION FOR TEXTURE

class GameScreenLevel2 : GameScreen
{
public:
	GameScreenLevel2(SDL_Renderer* renderer, int FP_S, int FP_L, int SP_S, int SP_L);
	~GameScreenLevel2();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	void UpdatePOWBlock(PowBlock* powB, Character* player, int player_id);
	void DoScreenShake();
	void SetLevelMap();
	void TileCollision();
	void CreateUI();
	void CreateText();
	void PlayerCollision(Character* player, int player_id);

private:

	int _FP_L;
	int _FP_S;
	int _SP_L;
	int _SP_S;

	bool m_screenshake;
	float m_shake_time;
	float m_wobble;
	float m_background_yPos;

	std::vector<PowBlock*> m_pow_block;
	std::vector<StaticBlock*> m_static_block;
	std::vector<Coin*> _coin;

	std::vector<UI*> _UI;

	Mix_Chunk* _s_coinCollect;
	Mix_Chunk* _s_powBlock;

	Vector2D my_char_pos;
	Vector2D my_char2_pos;
	Character* my_character;
	Character* my_character2;

	Texture2D* m_background_texture;

	Text* _text;
	Text* _text2;
	Text* _text3;
	int UI_offset = 630;

	bool SetUpLevel();


	LevelMap* m_level_map;


};


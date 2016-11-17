#pragma once
#include "BaseEngine.h"
#include "TileManager.h"
#include "Jwk04uTileManager.h"
#include "Jwk04uPlayer.h"

class Jwk04uMain :
	public BaseEngine
{
public:
	Jwk04uMain();
	~Jwk04uMain();

	void SetupBackgroundBuffer();
	int InitialiseObjects();
	void DrawStrings();
	void GameAction();
	void KeyDown(int iKeyCode);

	// State number
	enum State { stateInit, stateMain, statePaused, stateGameOver };
	
private:

	Jwk04uPlayer* playerController;

	State m_state;
	Jwk04uTileManager m_oTiles;


	int randEnemySelector;
	int highScore;
	int finalScore;


};


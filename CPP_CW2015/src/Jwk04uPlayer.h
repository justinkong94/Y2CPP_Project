#pragma once
#include "BaseEngine.h"
#include "DisplayableObject.h"
#include "Jwk04uBullet.h"
#include "Jwk04uEnemy.h"


class Jwk04uPlayer :
	public DisplayableObject
{
public:
	Jwk04uPlayer(BaseEngine* pEngine, int numberOfBullets, int numberOfEnemies, int enemySpawnTime, int bulletSpeed, int enemySpeed);
	~Jwk04uPlayer(void);
	void Draw();
	long Notify(int iSignalNumber);

	int currentScore;
	int currentHealth;

private:
	Jwk04uBullet* currentBullet;
	Jwk04uEnemy* currentEnemy;

	ImageData imPlayer;

	const int amountOfBullets;
	const int lastEnemyIndex;
	const int bulletTravelSpeed;
	
	int enemySpawnFrequency;
	int enemyTravelSpeed;
	
	int currentBulletIndex;
	bool isKeyCurrentlyPressed;

	int currentTime;
	int previousTime;
	
	int currentEnemyIndex;
	int randEnemyXPos;

public:
	void DoUpdate(int iCurrentTime);
};


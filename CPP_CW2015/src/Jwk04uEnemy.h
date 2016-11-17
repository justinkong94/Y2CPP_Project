#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"

class Jwk04uEnemy :
	public DisplayableObject
{
public:
	Jwk04uEnemy(BaseEngine* pEngine, int xPos, int yPos, int enemySpeed, int enemyType);
	~Jwk04uEnemy();
	void SetMovement(int iStartTime, int iEndTime, int iCurrentTime,
		int iStartX, int iStartY, int iEndX, int iEndY);
	void DoUpdate(int iCurrentTime);
	long Notify(int iSignalNumber);

protected:
	MovementPosition m_oMovement;

	const int storageXPos;
	const int storageYPos;
	const int enemyTravelSpeed;

	int storageEnemyType;
	int randXTarget;
	int randYTarget;

	double speed;

};

class Jwk04uEnemy1 :
	public Jwk04uEnemy
{
public:
	Jwk04uEnemy1(BaseEngine* pEngine, int xPos, int yPos, int enemySpeed, int enemyType);
	void Draw();

private:
	ImageData imEnemy1;
};

class Jwk04uEnemy2 :
	public Jwk04uEnemy
{
public:
	Jwk04uEnemy2(BaseEngine* pEngine, int xPos, int yPos, int enemySpeed, int enemyType);
	void Draw();

private:
	ImageData imEnemy2;
	
};


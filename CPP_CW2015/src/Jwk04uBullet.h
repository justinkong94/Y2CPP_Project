#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"


class Jwk04uBullet :
	public DisplayableObject
{
public:
	Jwk04uBullet(BaseEngine* pEngine,int numberOfBullets, int xPos, int yPos);
	~Jwk04uBullet();
	void Draw();
	void SetMovement(int iStartTime, int iEndTime, int iCurrentTime,
		int iStartX, int iStartY, int iEndX, int iEndY);
	void DoUpdate(int iCurrentTime);
	long Notify(int iSignalNumber);

private:
	ImageData imPlayerBullet;
	MovementPosition m_oMovement;
	const int storageXPos;
	const int storageYPos;
	const int amountOfBullets;
};


#include "header.h"
#include "JPGImage.h"
#include "templates.h"

#include "Jwk04uEnemy.h"

Jwk04uEnemy::Jwk04uEnemy(BaseEngine* pEngine, int xPos, int yPos, int enemySpeed, int enemyType)
	: DisplayableObject(pEngine)
	, storageXPos(xPos)
	, storageYPos(yPos)
	, enemyTravelSpeed(enemySpeed)
	, storageEnemyType(enemyType)
{
	
	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = xPos;
	m_iCurrentScreenY = m_iPreviousScreenY = yPos;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the ball size as both height and width
	m_iDrawWidth = 50;
	m_iDrawHeight = 50;
	// And make it visible
	SetVisible(false);

}

Jwk04uEnemy::~Jwk04uEnemy()
{
}

// Allows a caller to specify where the object will move from and to and when
void Jwk04uEnemy::SetMovement(int iStartTime, int iEndTime, int iCurrentTime,
	int iStartX, int iStartY, int iEndX, int iEndY)
{
	if (storageEnemyType == 2)
	{
		//generate random x target location
		randXTarget = rand() % (GetEngine()->GetScreenWidth() - 50);

		SetVisible(true);
		m_oMovement.Setup(iStartX, iStartY, randXTarget, iEndY, iStartTime, iEndTime);
		
	}
	//enemy type 1 goes here
	else{
		SetVisible(true);
		m_oMovement.Setup(iStartX, iStartY, iEndX, iEndY, iStartTime, iEndTime);
	}
	
	m_oMovement.Calculate(iCurrentTime);
	m_iCurrentScreenX = m_oMovement.GetX();
	m_iCurrentScreenY = m_oMovement.GetY();
}

// Handle a notification by a caller. The SignalNumber will have an application-specific meaning, 
long Jwk04uEnemy::Notify(int iSignalNumber) 
{ 
	//enemy moved to storage location and set invisible
	if (iSignalNumber == 1)
	{
		SetVisible(false);

		m_oMovement.Setup(storageXPos, storageYPos, storageXPos, storageYPos, GetEngine()->GetTime(), GetEngine()->GetTime());

		m_iCurrentScreenX = storageXPos;
		m_iCurrentScreenY = storageYPos;
	}

	return 0; 
}

void Jwk04uEnemy::DoUpdate(int iCurrentTime)
{
	// Work out current position
	m_oMovement.Calculate(iCurrentTime);
	m_iCurrentScreenX = m_oMovement.GetX();
	m_iCurrentScreenY = m_oMovement.GetY();

	// If movement has finished and enemy hit ground then reduce health
	if (m_oMovement.HasMovementFinished(iCurrentTime) && m_iCurrentScreenY >= (GetEngine()->GetScreenHeight() - 100) && IsVisible())
	{
		Notify(1);
		GetEngine()->NotifyAllObjects(3);
	}
	//enemy moved to storage location
	else if (m_oMovement.HasMovementFinished(iCurrentTime))
	{
		Notify(1);
	}
}

//enemy type 1 load image
Jwk04uEnemy1::Jwk04uEnemy1(BaseEngine* pEngine, int xPos, int yPos, int enemySpeed, int enemyType)
	: Jwk04uEnemy(pEngine, xPos, yPos, enemySpeed, enemyType)
{
	imEnemy1.LoadImage("enemy1.png");
}

void Jwk04uEnemy1::Draw()
{
	if (!IsVisible())
		return;

	imEnemy1.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, imEnemy1.GetWidth(), imEnemy1.GetHeight());

	// This will store the position at which the object was drawn
	// so that the background can be drawn over the top.
	// This will then remove the object from the screen.
	StoreLastScreenPositionForUndraw();
}


//enemy 2 load image
Jwk04uEnemy2::Jwk04uEnemy2(BaseEngine* pEngine, int xPos, int yPos, int enemySpeed, int enemyType)
	: Jwk04uEnemy(pEngine, xPos, yPos, enemySpeed,enemyType)
{
	imEnemy2.LoadImage("enemy2.png");
}

void Jwk04uEnemy2::Draw()
{
	if (!IsVisible())
		return;

	imEnemy2.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, imEnemy2.GetWidth(), imEnemy2.GetHeight());

	// This will store the position at which the object was drawn
	// so that the background can be drawn over the top.
	// This will then remove the object from the screen.
	StoreLastScreenPositionForUndraw();
}


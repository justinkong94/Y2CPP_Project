#include "header.h"
#include "JPGImage.h"
#include "Jwk04uPlayer.h"
#include "Jwk04uBullet.h"
#include "Jwk04uMain.h"
#include "BaseEngine.h"


Jwk04uPlayer::Jwk04uPlayer(BaseEngine* pEngine, int numberOfBullets, int numberOfEnemies, int enemySpawnTime, int bulletSpeed, int enemySpeed)
	: DisplayableObject(pEngine)
	,currentBullet(NULL) 
	,amountOfBullets(numberOfBullets) 
	,lastEnemyIndex(numberOfEnemies + numberOfBullets)
	, bulletTravelSpeed(bulletSpeed)
{
	currentTime = 0;
	previousTime = 0;
	currentScore = 0;
	currentHealth = 3; // 3 health
	enemyTravelSpeed = enemySpeed;
	enemySpawnFrequency = enemySpawnTime;

	currentBulletIndex = 1;
	isKeyCurrentlyPressed = false;

	currentEnemyIndex = amountOfBullets + 1;
	
	// Load the image file into an image object - at the normal size
	imPlayer.LoadImage("player.png");

	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = GetEngine()-> GetScreenWidth() /2;
	m_iCurrentScreenY = m_iPreviousScreenY = GetEngine()->GetScreenHeight() - 100;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the ball size as both height and width
	m_iDrawWidth = 50;
	m_iDrawHeight = 50;
	// And make it invisible
	SetVisible(false);

}


Jwk04uPlayer::~Jwk04uPlayer()
{
}


void Jwk04uPlayer::Draw()
{
	if (!IsVisible())
		return;

	imPlayer.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY,
		imPlayer.GetWidth(), imPlayer.GetHeight());

	// This will store the position at which the object was drawn
	// so that the background can be drawn over the top.
	// This will then remove the object from the screen.
	StoreLastScreenPositionForUndraw();

}

long Jwk04uPlayer::Notify(int iSignalNumber)
{
	//game over call
	if (iSignalNumber == -1)
	{
		//call game over state
		GetEngine()->KeyDown(SDLK_SLEEP);
	}
	else if (iSignalNumber == 0)
	{
		//add score
		currentScore++;

		//increase enemy travel speed and spawn speed every 20 score achieved
		if (currentScore % 20 == 0 && currentScore > 0 && currentScore < 300)
		{
			enemyTravelSpeed -= 250;
			enemySpawnFrequency -= 50;
		}
	}
	else if (iSignalNumber == 2)
	{
		//set visible
		SetVisible(true);
	}
	//Decrease health
	else if (iSignalNumber == 3)
	{
		currentHealth--;

		//0 health reached, player dies
		if (currentHealth == 0)
		{
			//call end game state
			Notify(-1);
		}
	}

	return 0;
}

void Jwk04uPlayer::DoUpdate(int iCurrentTime)
{
	currentTime = iCurrentTime / enemySpawnFrequency;

	//spawn a new enemy at a fixed rate
	if (currentTime > previousTime)
	{
		//gets a random x start position for enemy
		randEnemyXPos = rand() % (GetEngine()->GetScreenWidth() - 50);

		currentEnemy = dynamic_cast<Jwk04uEnemy*>(GetEngine()->GetDisplayableObject(currentEnemyIndex));
	
		//start enemy movement
		currentEnemy->SetMovement(GetEngine()->GetTime(), GetEngine()->GetTime() + enemyTravelSpeed, GetEngine()->GetTime(),
			randEnemyXPos, 0, randEnemyXPos, GetEngine()->GetScreenHeight() - 100);
		
		//go to next enemy in array
		if (currentEnemyIndex < lastEnemyIndex){
			currentEnemyIndex++;
		}
		//go back to first enemy in array
		else{
			currentEnemyIndex = amountOfBullets + 1;
		}

		previousTime = currentTime;
	}

	// Change position if player presses a key
	if (GetEngine()->IsKeyPressed(SDLK_LEFT)){
		m_iCurrentScreenX -= 10;
	}
		
	if (GetEngine()->IsKeyPressed(SDLK_RIGHT)){
		m_iCurrentScreenX +=10;
	}

	//shoots bullets
	if (GetEngine()->IsKeyPressed(SDLK_SPACE)){

		if(isKeyCurrentlyPressed == false){

			currentBullet = dynamic_cast<Jwk04uBullet*>(GetEngine()->GetDisplayableObject(currentBulletIndex));

			//set bullet movement
			currentBullet->SetMovement(GetEngine()->GetTime(), GetEngine()->GetTime() + bulletTravelSpeed,
				GetEngine()->GetTime(), m_iCurrentScreenX + (m_iDrawWidth / 2) - 5, m_iCurrentScreenY - 20, 
				m_iCurrentScreenX + (m_iDrawWidth / 2) - 5, 0);

			isKeyCurrentlyPressed = true;

			//shuffle through bullet array
			if (currentBulletIndex < amountOfBullets){
				currentBulletIndex++;
			}
			else{
				currentBulletIndex = 1;
			}
		}

	} else{
		isKeyCurrentlyPressed = false;
	}

	//prevents player from moving off screen
	if (m_iCurrentScreenX < 0)
		m_iCurrentScreenX = 0;
	if (m_iCurrentScreenX >= GetEngine()->GetScreenWidth() - m_iDrawWidth)
		m_iCurrentScreenX = GetEngine()->GetScreenWidth() - m_iDrawWidth;


	/********** EXTRA STUFF FOR COLLISIONS ********/

	// Iterate through the objects
	// We are looking for one which is too close to us
	DisplayableObject* pObject;

	for (int iObjectId = amountOfBullets + 1;
		(pObject = GetEngine()->GetDisplayableObject(iObjectId)
		) != NULL;
	iObjectId++)
	{
		if (pObject == this) // This is us, skip it
			continue;
		
		int iXDiff = pObject->GetXCentre() - m_iCurrentScreenX;
		int iYDiff = pObject->GetYCentre() - m_iCurrentScreenY;

		// Estimate the size - by re-calculating it
		int iTick = iCurrentTime / 20; // 1 per 20ms
		int iFrame = iTick % 30;
		int iSize = 10 + iFrame;
		if (iFrame > 15)
			iSize = 10 + (30 - iFrame);
		int iSizeOther = iSize; // Assume both the same size

		// Pythagorus' theorum:
		if (((iXDiff*iXDiff) + (iYDiff*iYDiff))
			< ((iSizeOther + iSize)*(iSizeOther + iSize)))
		{
			//moves enemy to storage position and set invisible
			pObject->Notify(1);

			//call add score function
			Notify(3);
			
			// Ensure that the object gets redrawn on the display, if something changed
			RedrawObjects();
			return;
		}
	}

	// Ensure that the object gets redrawn on the display, if something changed
	RedrawObjects();

}



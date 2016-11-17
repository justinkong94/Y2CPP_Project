#include "header.h"
#include "JPGImage.h"
#include "templates.h"

#include "Jwk04uBullet.h"


Jwk04uBullet::Jwk04uBullet(BaseEngine* pEngine, int numberOfBullets,int xPos, int yPos)
	: DisplayableObject(pEngine)
	, amountOfBullets(numberOfBullets)
	, storageXPos(xPos)
	, storageYPos(yPos)
{
	imPlayerBullet.LoadImage("playerBullet.png");

	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = xPos;
	m_iCurrentScreenY = m_iPreviousScreenY = yPos;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the ball size as both height and width
	m_iDrawWidth = 10;
	m_iDrawHeight = 20;
	// And make it visible
	SetVisible(false);

}

Jwk04uBullet::~Jwk04uBullet()
{
}


void Jwk04uBullet::Draw()
{
	if (!IsVisible())
		return;

	imPlayerBullet.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, imPlayerBullet.GetWidth(), imPlayerBullet.GetHeight());

	// This will store the position at which the object was drawn
	// so that the background can be drawn over the top.
	// This will then remove the object from the screen.
	StoreLastScreenPositionForUndraw();

}

// Allows a caller to specify where the object will move from and to and when
void Jwk04uBullet::SetMovement(int iStartTime, int iEndTime, int iCurrentTime,
	int iStartX, int iStartY, int iEndX, int iEndY)
{
	SetVisible(true);
	m_oMovement.Setup(iStartX, iStartY, iEndX, iEndY, iStartTime, iEndTime);
	m_oMovement.Calculate(iCurrentTime);
	m_iCurrentScreenX = m_oMovement.GetX();
	m_iCurrentScreenY = m_oMovement.GetY();
}

long Jwk04uBullet::Notify(int iSignalNumber){

	if (iSignalNumber == 1)
	{
		SetVisible(false);

		m_oMovement.Setup(storageXPos, storageYPos, storageXPos, storageYPos, GetEngine()->GetTime(), GetEngine()->GetTime());
		
		m_iCurrentScreenX = storageXPos;
		m_iCurrentScreenY = storageYPos;

	}

	return 0;
};

void Jwk04uBullet::DoUpdate(int iCurrentTime)
{

	// Work out current position
	m_oMovement.Calculate(iCurrentTime);
	m_iCurrentScreenX = m_oMovement.GetX();
	m_iCurrentScreenY = m_oMovement.GetY();

	// If movement has finished then request instructions
	if (m_oMovement.HasMovementFinished(iCurrentTime))
	{
		Notify(1);
	}

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
		
		// We are just using base class parts
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
			pObject->Notify(1);
			Notify(1);
			GetEngine()->NotifyAllObjects(0);

			// Ensure that the object gets redrawn on the display, if something changed
			RedrawObjects();
			return;
		}
	}

	// Ensure that the object gets redrawn on the display, if something changed
	RedrawObjects();
}

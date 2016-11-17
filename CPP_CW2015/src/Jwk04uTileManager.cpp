#include "header.h"
#include "Jwk04uTileManager.h"
#include "JPGImage.h"



Jwk04uTileManager::Jwk04uTileManager()
	: TileManager(50, 50)
{
	imGrass.LoadImage("grass.png");
	imHealth.LoadImage("health.png");
}


Jwk04uTileManager::~Jwk04uTileManager()
{
}

// Draw a tile at a location on the screen determined by the tile
// position and the base X and Y co-ordinates
void Jwk04uTileManager::DrawTileAt(
	BaseEngine* pEngine,
	SDL_Surface* pSurface,
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY) const
{

	// Load the image file into an image object - at the normal size
	imGrass.RenderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, imGrass.GetWidth(), imGrass.GetHeight());
	
}

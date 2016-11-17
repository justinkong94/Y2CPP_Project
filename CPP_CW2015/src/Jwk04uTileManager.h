#pragma once
#include "JPGImage.h"
#include "TileManager.h"
class Jwk04uTileManager :
	public TileManager
{
public:
	Jwk04uTileManager();
	~Jwk04uTileManager();

	// Draw a tile at a location on the screen determined by the tile
	// position and the base X and Y co-ordinates
	virtual void DrawTileAt(
		BaseEngine* pEngine,
		SDL_Surface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const;

private:
	ImageData imGrass;
	ImageData imHealth;
};


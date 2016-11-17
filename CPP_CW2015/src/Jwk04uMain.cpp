#include "header.h"
#include "Jwk04uMain.h"
#include "JPGImage.h"
#include "TileManager.h"
#include "Jwk04uPlayer.h"
#include "Jwk04uBullet.h"
#include "Jwk04uEnemy.h"
#include <sstream>  // Required for stringstreams
#include <string> 

#include <iostream>
#include <fstream>
using namespace std;


#define NUMBEROFBULLETS 10
#define BULLETSTORAGEXPOS 0
#define BULLETSTORAGEYPOS GetScreenHeight()- 40

#define MAXENEMIES 20
#define ENEMYSTORAGEXPOS GetScreenWidth() - 50
#define ENEMYSTORAGEYPOS GetScreenHeight()- 50

//in miliseconds
#define ENEMYSPAWNTIME 1000
//time taken for enemy to travel from top to bottom of the screen
#define ENEMYSPEED 5000
//time taken for bullet to travel from bottom to top of the screen
#define BULLETSPEED 1000

string score;
string health;
string highScoreString;

Jwk04uMain::Jwk04uMain()
	: BaseEngine(50)
	, m_state(stateInit)
{
	//read high score from file
	string line;
	ifstream myfile("gameData.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			highScore = atoi(line.c_str());
		}
		myfile.close();
	}

	else cout << "Unable to open file";
}


Jwk04uMain::~Jwk04uMain()
{
}

/* Fill the background - should be overridden to actually draw the correct thing. */
void Jwk04uMain::SetupBackgroundBuffer()
{
	ofstream myfile;
	// Draw an image loaded from a file.
	ImageData imStar1, imStar2, imStar3, imStar4, imMainPlayer, iHealth;
	// NEW SWITCH
	switch (m_state)
	{
	case stateInit: // Reload the level data
		FillBackground(0x0000FF);

		imMainPlayer.LoadImage("player.png");
		imMainPlayer.RenderImageWithMask(this->GetBackground(), 0, 0, GetScreenWidth() / 2 - 20, GetScreenHeight() / 2 - 30, imMainPlayer.GetWidth(), imMainPlayer.GetHeight());
		
		break;

	case stateMain:
		FillBackground(0X10101c);
		{
			for (int iX = 0; iX < GetScreenWidth(); iX++){
				for (int iY = 0; iY < this->GetScreenHeight(); iY++){
					switch (rand() % 500){
					case 0:
						SetBackgroundPixel(iX, iY, 0X283a62);
						break;
					case 1:
						SetBackgroundPixel(iX, iY, 0Xfdfef8);
						break;
					default:
						break;
					}
				}
			}

			// Load the image file into an image object - at the normal size
			imStar1.LoadImage("star1.png");
			imStar2.LoadImage("star2.png");
			imStar3.LoadImage("star3.png");
			imStar4.LoadImage("star4.png");


			for (int x = 0; x < GetScreenWidth() - (GetScreenWidth() % 20); x += 20){
				for (int y = 0; y < GetScreenHeight() - (GetScreenHeight() % 20); y += 20){

					switch (rand() % 150)
					{
					case 0:
						imStar1.RenderImageWithMask(this->GetBackground(), 0, 0, x, y, imStar1.GetWidth(), imStar1.GetHeight());
						break;
					case 1:
						imStar2.RenderImageWithMask(this->GetBackground(), 0, 0, x, y, imStar2.GetWidth(), imStar2.GetHeight());
						break;
					case 2:
						imStar3.RenderImageWithMask(this->GetBackground(), 0, 0, x, y, imStar3.GetWidth(), imStar3.GetHeight());
						break;
					case 3:
						imStar4.RenderImageWithMask(this->GetBackground(), 0, 0, x, y, imStar4.GetWidth(), imStar4.GetHeight());
						break;
					default:
						break;
					}
				}
			}

			// Specify the screen x,y of top left corner, 50 is tile height
			m_oTiles.SetBaseTilesPositionOnScreen(0, GetScreenHeight() - 50);
			// Tell it to draw tiles from x1,y1 to x2,y2 in tile array,
			// to the background of this screen
			m_oTiles.DrawAllTiles(this, this->GetBackground(), 0, 0, 17, 0);
		}
		break; 
	case stateGameOver: // Game over screen
		FillBackground(0X64FE2E);

		finalScore = playerController->currentScore;// get final score of game

		if (finalScore > highScore)
		{
			highScore = finalScore;
			myfile.open("gameData.txt");//write high score to file
			myfile << highScore;
			myfile.close();
		}

		highScoreString = "High Score: " + std::to_string(highScore);
		break;
	default:
		break;
	} // End switch	
}


int Jwk04uMain::InitialiseObjects()
{

	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	DrawableObjectsChanged();
	// Destroy any existing objects
	DestroyOldObjects();
	// Creates an array to store the objects
	// Needs to have room for the NULL at the end
	CreateObjectArray(NUMBEROFBULLETS + MAXENEMIES + 2);

	StoreObjectInArray(0, new Jwk04uPlayer(this, NUMBEROFBULLETS, MAXENEMIES, ENEMYSPAWNTIME, BULLETSPEED, ENEMYSPEED));

	for (int i = 1; i <= NUMBEROFBULLETS; i++)
	{
		StoreObjectInArray(i, new Jwk04uBullet(this, NUMBEROFBULLETS, BULLETSTORAGEXPOS, BULLETSTORAGEYPOS));
	}

	for (int j = NUMBEROFBULLETS + 1; j <= NUMBEROFBULLETS + MAXENEMIES; j++)
	{
		randEnemySelector = rand() % 10;

		if (randEnemySelector >= 0 && randEnemySelector < 6)
		{
			StoreObjectInArray(j, new Jwk04uEnemy1(this, ENEMYSTORAGEXPOS, ENEMYSTORAGEYPOS,ENEMYSPEED, 1));
		}
		else 
		{
			StoreObjectInArray(j, new Jwk04uEnemy2(this, ENEMYSTORAGEXPOS, ENEMYSTORAGEYPOS,ENEMYSPEED, 2));
		}
	}

	StoreObjectInArray(NUMBEROFBULLETS + MAXENEMIES + 1, NULL);

	playerController = dynamic_cast<Jwk04uPlayer*>(GetDisplayableObject(0));

	return 0;
}

/* Draw text labels */
void Jwk04uMain::DrawStrings()
{
	score = std::to_string(playerController->currentScore);
	score = "Score: " + score;

	health = std::to_string(playerController->currentHealth);
	health = "Lives: " + health;

	const char *charScore = score.c_str();
	const char *charHealth = health.c_str();
	const char *highScoreChar = highScoreString.c_str();
	
	// NEW SWITCH
	switch (m_state)
	{
	case stateInit:
		CopyBackgroundPixels(0/*X*/, 0/*Y*/, GetScreenWidth(), GetScreenHeight()/*Height*/);
		DrawScreenString(330, 150, "ALIEN DEFENDER", 0x0, NULL);
		DrawScreenString(230, 450, "Press TAB to start the game", 0x0, NULL);
		break;
	case stateMain:
		CopyBackgroundPixels(0/*X*/, GetScreenHeight() - 40/*Y*/, GetScreenWidth(), 40/*Height*/);
		DrawScreenString(50, GetScreenHeight() - 40, charScore, 0xffffff, NULL);
		DrawScreenString(GetScreenWidth() - 150, GetScreenHeight() - 40, charHealth, 0xffffff, NULL);
		break;
	case stateGameOver: // Game over screen
		CopyBackgroundPixels(0/*X*/, 0/*Y*/, GetScreenWidth(), GetScreenHeight()/*Height*/);
		DrawScreenString(200, 200, highScoreChar, 0xffffff, NULL);
		DrawScreenString(200, 250, charScore, 0xffffff, NULL);
		DrawScreenString(200, 300, "Gave Over. Press TAB to restart", 0xffffff, NULL);
		break;
	default:
		break;
	}
}

/* Overridden GameAction to ensure that objects use the modified time */
void Jwk04uMain::GameAction()
{
	// If too early to act then do nothing
	if (!IsTimeToAct()) // No sleep, will just keep polling constantly 
		return;

	// Don't act for another 15 ticks
	SetTimeToAct(15);

	// NEW SWITCH 
	switch (m_state)
	{
	case stateInit:
		break;
	case stateMain:
		// Only tell objects to move when not paused etc
		UpdateAllObjects(GetModifiedTime());
		break;
	}
}

//key presses are handled here
void Jwk04uMain::KeyDown(int iKeyCode)
{
	// NEW SWITCH on current state
	switch (iKeyCode)
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		SetExitWithCode(0);
		break;
	case SDLK_TAB: // TAB Pauses
		switch (m_state)
		{
		case stateInit:
			// Go to state main
			m_state = stateMain;
			// Force redraw of background
			SetupBackgroundBuffer();
			InitialiseObjects();
			NotifyAllObjects(2);

			// Redraw the whole screen now
			Redraw(true);
			break;
		case stateGameOver:
			// Go to state main
			m_state = stateInit;

			// Force redraw of background
			SetupBackgroundBuffer();
			// Redraw the whole screen now
			Redraw(true);
			break;
		} // End switch on current state
		break; // End of case TAB
	case SDLK_SLEEP:
		m_state = stateGameOver;

		DestroyOldObjects();
		// Force redraw of background
		SetupBackgroundBuffer();
		// Redraw the whole screen now
		Redraw(true);
		break;
		break;
	default:
		break;
	}
}





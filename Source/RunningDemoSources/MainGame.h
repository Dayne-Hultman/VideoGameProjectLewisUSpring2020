#ifndef __MAINGAME_H__
#define __MAINGAME_H__

#include "Frog.h"
#include "MenuState.h"

//#include "CollisionCheck.h"

namespace Webfoot {

class Player;
class Tile;
class JumpableObstacles;

//==============================================================================

class MainGame : public MenuState
{
public:
   typedef MenuState Inherited;

   MainGame();
   virtual ~MainGame() {};
   
   virtual void Init();
   virtual void OnGUILayerInit(LayerWidget* layer);
   virtual void Deinit();
   virtual void Update();
   virtual void Draw();

   static MainGame instance;

protected:
   virtual const char* GUILayerNameGet();

   /// Called when the Pause button is clicked.
   static void OnPauseClick(PressButtonWidget*, void*);
   /// Called when the Winged Shoes button is clicked.
   static void OnShoeClick(PressButtonWidget*, void*);
   /// Called when the Multiplier button is clicked.
   static void OnMultiplyClick(PressButtonWidget*, void*);
   /// Called when the Magnet button is clicked.
   static void OnMagnetClick(PressButtonWidget*, void*);

   /// Called when the Main Menu button is clicked.
   static void OnMenuClick(PressButtonWidget*, void*);
   /// Called when the Rerun button is clicked.
   static void OnRerunClick(PressButtonWidget*, void*);

   NumberLabelWidget* goldLabel;
   NumberLabelWidget* distanceLabel;
   NumberLabelWidget* multiplyAmountLabel;
   NumberLabelWidget* magnetAmountLabel;
   NumberLabelWidget* shoeAmountLabel;
   NumberLabelWidget* goldEndLabel;
   NumberLabelWidget* distanceEndLabel;

   Sprite *hourglass_sprite;
   Sprite *game_over_sprite;

   Tile* Background, *Background1;
   Player* player;
   Tile* tile;
   Tile* tile1;
   Tile* tile2;
   Tile* tile3;
   Tile* tiles[4];

   Tile* coins[10];

   Tile* jObst[7];

   bool tileSurface = false;
   bool paused = false;
   bool obstacleJustPlaced = false;
   bool gameOver = false;

   float obstacleJustPlacedX;
   int goldCount = 0;
   int distance = 0;
   int dist = 0;
   int multiplyCount = 0;
   int magnetCount = 0;
   int shoeCount = 0;
   int minDistNextCoinPlaced = 0;
   int minDistNextObstPlaced = 0;
   int coinsOnObstacle = 0;


   Table<Sound*> sounds; /// Table of all sounds



   
};

MainGame* const theMainGame = &MainGame::instance;

class Player
{
public:
	Player();
	
	void Init();
	void Deinit();
	void Update(bool surface, unsigned int dt);
	void Draw();
	void coinNoise();


//protected:
	Sprite *mysprite;
	Point2F position;
	Point2F velocity, accel;
	float height;
	bool onSurface;
	bool paused = false;

};

class Tile
{
public:
	Tile();
	
	void Init(float xPos, float yPos, float xVel, Image* image, bool isCoin, bool isVisible);
	void Deinit();
	void Update(unsigned int dt, bool newCoin = false, bool newObstacle = false);
	void Draw();
	void CollisionCheck(Player* player, unsigned int dt);

	float getWidth();
	float getHeight();
	Point2F getPosition();

	Point2F position;
	bool onSurface;
	float xVelocity;
	bool goldScored = false;
	bool obstacle = false;
	bool visible = true;
	bool obstacleCollision = false;
	float coinHeight;


protected:
	Image* image;
	
	float height;
	float width;
	bool paused = false;
	bool coin;
	
	bool enemy = false;

};

class JumpableObstacles
{
public:
	JumpableObstacles();

	void Init(float xPos, float yPos, float xVel, Image* image);
	void Deinit();
	void Update(unsigned int dt);
	void Draw();
	void CollisionCheck(Player* player, unsigned int dt);

	float getWidth();
	float getHeight();
	Point2F getPosition();
	bool onSurface;
	float xVelocity;

protected:
	Image* image;
	Point2F position;
	float height;
	float width;



};
} //namespace Webfoot {

#endif //#ifndef __MAINGAME_H__

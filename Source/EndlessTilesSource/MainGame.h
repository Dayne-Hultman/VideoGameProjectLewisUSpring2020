#ifndef __MAINGAME_H__
#define __MAINGAME_H__

#include "Frog.h"
#include "MenuState.h"
//#include "CollisionCheck.h"

namespace Webfoot {

class Player;
class Tile;
class Ground;

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

   /// Called when the Jump button is clicked.
   static void OnJumpClick(PressButtonWidget*, void*);
   /// Called when the Shoot button is clicked.
   static void OnShootClick(PressButtonWidget*, void*);
   /// Called when the Slide button is clicked.
   static void OnSlideClick(PressButtonWidget*, void*);
   /// Called when the Slow button is clicked.
   static void OnSlowClick(PressButtonWidget*, void*);
   /// Called when the Pause button is clicked.
   static void OnPauseClick(PressButtonWidget*, void*);

   Image* Background;
   Player* player;
   Ground* ground;
   bool tileSurface = false;
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


//protected:
	Sprite *mysprite;
	Point2F position;
	Point2F velocity, accel;
	float height;
	bool onSurface;

};

class Tile
{
public:
	Tile();
	
	void Init(float height, float width, float xPos, float yPos, float xVel);
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

class Ground
{
public:
	Ground();

	void Init();
	void Deinit();
	void Update(unsigned int dt, Player* playerptr);
protected:
	int Width;
	int xx;
	Tile tiles[4];
	Tile tile;
	Tile tile1;
	Tile tile2;
	Tile tile3;

};

class Coin
{
public:
	void Init();
	void Deinit();
	void Update(unsigned int dt);
	void Draw();
protected:
	Sprite* mysprite;
	Point2F position;
	float height;
	float width;
}; 

class Enemy
{
public:
	void Init();
	void Deinit();
	void Update(unsigned int dt);
	void Draw();
protected:
	int level;
	Sprite* mysprite;
	float height;
	float width;
	Point2F position;
};

} //namespace Webfoot {

#endif //#ifndef __MAINGAME_H__

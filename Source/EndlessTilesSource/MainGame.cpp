#include "Frog.h"
#include "MainGame.h"
#include "MainUpdate.h"

using namespace Webfoot;

MainGame MainGame::instance;

//==============================================================================

/// Main GUI
#define GUI_LAYER_NAME "MainGame"

//-----------------------------------------------------------------------------

MainGame::MainGame()
{
   player = NULL;
   ground = NULL;
   Background = NULL;
}

//-----------------------------------------------------------------------------

void MainGame::Init()
{
   Inherited::Init();

   Background = theImages->Load("DaySky");

   player = frog_new Player();
   player->Init();

   ground->Init();
}

//-----------------------------------------------------------------------------
void MainGame::OnGUILayerInit(LayerWidget*)
{
	// Set up the GUI callbacks.
	PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Jump", OnJumpClick);
}

void MainGame::Deinit()
{
   if(player)
   {
	   player->Deinit();
	   frog_delete player;
	   player = NULL;
   }
   if (Background)
   {
	   theImages->Unload(Background);
	   Background = NULL;
   }
   Inherited::Deinit();
}

//-----------------------------------------------------------------------------

const char* MainGame::GUILayerNameGet()
{
   return GUI_LAYER_NAME;
}

//-----------------------------------------------------------------------------
void MainGame::OnJumpClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainGame::OnShootClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainGame::OnSlowClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainGame::OnSlideClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainGame::OnPauseClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainGame::Update()
{
   Inherited::Update();

   unsigned int dt = theClock->LoopDurationGet();

   ground->Update(dt, player);

   if(!theStates->StateChangeCheck() && theKeyboard->KeyJustPressed(KEY_ESCAPE))
   {
      theMainGame->StateChangeTransitionBegin(true);
      theStates->Pop();
   }
}

//-----------------------------------------------------------------------------

void MainGame::Draw()
{
	Background->Draw();
	player->Draw();
}

//==============================================================================

Player::Player()
{
   mysprite = NULL;
}

//------------------------------------------------------------------------------

void Player::Init()
{
	height = 64.0f;
	mysprite = frog_new Sprite();
	mysprite->VisibleSet(true);
	mysprite->Init("Sprites/Player", "Player");
	position = Point2F::Create(theScreen->SizeGet()/2.0f);

	position.y = theScreen->HeightGet()/4.0f;
	DebugPrintf("starting height: %f\n", position.y);
	mysprite->PositionSet(position);
	mysprite->AnimationSet("Player");
	onSurface = false;
	velocity = Point2F::Create(0.0f, 0.0f);
	accel = Point2F::Create(0.0f, 0.0f);

}

//------------------------------------------------------------------------------

void Player::Deinit()
{

   if(mysprite)
   {
	   mysprite->Deinit();
	   frog_delete mysprite;
	   mysprite = NULL;
   }
}

//------------------------------------------------------------------------------

void Player::Update(bool surface, unsigned int dt)
{
   float dtSeconds = (float)dt / 1000.0f;
   //DebugPrintf("x: %f, y: %f \n", position.x, position.y);
   //DebugPrintf("accelY: %f\n", accel.y);
   onSurface = surface;
   if (onSurface){
	   accel.y = 0.0f;
	   velocity.y = 0.0f;
   }
   else{
	   accel.y = .01f;
   }
   
   
   if (position.y > (2*theScreen->HeightGet())){
	   position = Point2F::Create(theScreen->SizeGet() / 2.0f);
	   position.y = theScreen->HeightGet() / 4.0f;
   }
   if (velocity.y > 2.0f){
	   velocity.y = 2.0f;
   }
   
   

   if (theKeyboard->KeyPressed(KEY_D)){
	   if (position.x > theScreen->WidthGet()){
		   position.x = -10.0f;
	   }
	   else if (position.x > -11.0f){
		   position.x += 0.3f * dt;
	   }
   }
   if (theKeyboard->KeyPressed(KEY_A)){
	   if (position.x < -10.0f){
		   position.x = theScreen->WidthGet();
	   }
	   else if (position.x < (theScreen->WidthGet() + 1.0f)){
		   position.x -= 0.3f * dt;
	   }
   }
   if (theKeyboard->KeyJustPressed(KEY_SPACE)){
	   if (onSurface){
		   velocity.y = -2.5f;

	   }
   }

   velocity += accel * float(dt);
   position += velocity * float(dt);

   mysprite->PositionSet(position);
   mysprite->Update(dt);
}

//------------------------------------------------------------------------------

void Player::Draw()
{

	mysprite->Draw();
}

Tile::Tile()
{
	image = NULL;
}

//------------------------------------------------------------------------------

void Tile::Init(float w, float h, float xPos, float yPos, float xVel)
{
	image = theImages->Load("TileCobblestone");

	height = h;
	width = w;
	height = image->HeightGet();
	width = image->WidthGet();
	DebugPrintf("height: %f, width: %f\n", height, width);
	position.x = xPos;
	position.y = yPos;
	xVelocity = xVel;

	onSurface = false;

}

//------------------------------------------------------------------------------

void Tile::Deinit()
{

	if (image)
	{
		theImages->Unload(image);
		image = NULL;
	}
}

//------------------------------------------------------------------------------

void Tile::Update(unsigned int dt)
{
	float dtSeconds = (float)dt / 1000.0f;
	position.x -= xVelocity * dt;
	if (position.x + width / 2.0f < 0.0f){
		position.x = theScreen->SizeGet().x + width / 2.0f;
	}
}

//------------------------------------------------------------------------------

void Tile::Draw()
{

	image->Draw(position - (Point2F::Create(image->SizeGet()) / 2.0f));
}

//------------------------------------------------------------------------------

void Tile::CollisionCheck(Player* player, unsigned int dt){

	//DebugPrintf("player y pos: %f ,size of sprite y: %f, tile y pos: %f\n", player->position.y, player->mysprite->SizeGet(dt).y, position.y);
	//DebugPrintf("player x pos: %f ,size of sprite x: %f, tile x pos: %f, tile x pos + width: %f\n", player->position.x, player->mysprite->SizeGet(dt).x, position.x, position.x + width);
	//DebugPrintf("sprite height: %f, player height: %f\n", player->mysprite->SizeGet(dt).y, player->height);

	Point2F playerPos = Point2F::Create(player->position);
	Point2F playerSprite = Point2F::Create(player->mysprite->SizeGet(dt));
	Point2F playerVel = Point2F::Create(player->velocity);

	if (((playerPos.x + playerSprite.x / 2.0f) - 13.0f > position.x - width / 2.0f) && (playerPos.x - playerSprite.x / 2.0f + 13.0f< (position.x + (width / 2.0)))){
		//DebugPrintf("Collision on x\n");
		//if (player->position.y + player->mysprite->SizeGet(dt).y/2.0f + 10.0f > position.y - (height / 2.0f) && (player->position.y + player->mysprite->SizeGet(dt).y/2.0f - 10.0f < position.y - (height / 2.0f))){
		if (playerPos.y + playerSprite.y / 2.0f > position.y - (height / 2.0f) - 1.0f && playerPos.y + playerSprite.y / 2.0f < (position.y - height / 3.0f) && (playerVel.y >= 0)){
			onSurface = true;
			player->position.y = position.y - height / 2.0f - playerSprite.y / 2.0f;
		}
		else if (playerPos.y - playerSprite.y / 2.0f < position.y + (height / 2.0f) && (playerPos.y - playerSprite.y / 2.0f > position.y + height / 3.0f) && playerVel.y < 0){
			player->velocity.y = 0.0f;
			DebugPrintf("Collision from bottom\n");
		}
		else{
			onSurface = false;
		}
	}
	else{
		onSurface = false;
	}
	if ((playerPos.y + playerSprite.y / 2.0f > (position.y - height / 2.0f + 2.0f) && (playerPos.y + playerSprite.y / 2.0f < (position.y + height / 2.0f - 2.0f))) || (playerPos.y - playerSprite.y / 2.0f < (position.y + height / 2.0f - 2.0f) && (playerPos.y - playerSprite.y / 2.0f > (position.y - height / 2.0f + 2.0f)))){
		//DebugPrintf("Collision from side\n");
		if ((playerPos.x + playerSprite.x / 2.0f - 13.0f > position.x - width / 2.0f) && (playerPos.x + playerSprite.x / 2.0f - 13.0f < position.x - width / 2.0f + 20.0f)){
			DebugPrintf("Collision from left\n");
			player->position.x = position.x - width / 2.0f - playerSprite.x / 2.0f + 12.0f;
		}
		else if ((playerPos.x - playerSprite.x / 2.0f + 13.0f < position.x + width / 2.0f) && (playerPos.x - playerSprite.x / 2.0f + 13.0f > position.x + width / 2.0 - 20.0f)){
			DebugPrintf("Collision from right\n");
			player->position.x = position.x + width / 2.0f + playerSprite.x / 2.0f - 12.0f;
		}
	}
	

}

//------------------------------------------------------------------------------

float Tile::getWidth(){
	return width;
}

//------------------------------------------------------------------------------

float Tile::getHeight(){
	return height;
}

//------------------------------------------------------------------------------

Point2F Tile::getPosition(){
	return position;
}

Ground::Ground()
{
}

//------------------------------------------------------------------------------

void Ground::Init()
{ 
	xx = 0;
	Width = theScreen->WidthGet();
	for (int i = 0; i < 4; i++)
	{
		tiles[i].Init(480.0f, 130.0f, xx, theScreen->HeightGet() - 200.0f, 0.15f);
		tiles[i].Draw();
		xx = xx + 480;
	}
}

//------------------------------------------------------------------------------

void Ground::Deinit()
{
	for (int i = 0; i < 4; i++)
	{
		tiles[i].Deinit();
	}
}

void Ground::Update(unsigned int dt, Player* playerptr)
{
	for (int i = 0; i < 4; i++){
		tiles[i].Update(dt);
		if (tiles[i].getPosition().x + 480.0f < 0){
			tiles[i].Deinit();
			tiles[i].Init(480.0f, 130.0f, xx, theScreen->HeightGet() - 200.0f, 0.15f);
		}
	}
}

	

#include "Frog.h"
#include "MainGame.h"
#include "MainUpdate.h"
#include "Sound.h"


using namespace Webfoot;

MainGame MainGame::instance;

///Sounds
const char* SOUND_PATHS[] =
{
	"Sounds/button",
	"Sounds/coin",
	"Sounds/death",
	"Sounds/jump"
};

enum Sounds
{
	SOUND_BUTTON,
	SOUND_COIN,
	SOUND_DEATH,
	SOUND_JUMP,
  SOUND_COUNT
};

//==============================================================================

/// Main GUI
#define GUI_LAYER_NAME "MainGame"

//-----------------------------------------------------------------------------

MainGame::MainGame()
{
   player = NULL;
   tile = NULL;
   Background = NULL;
   goldLabel = NULL;
   distanceLabel = NULL;
   multiplyAmountLabel = NULL;
   magnetAmountLabel = NULL;
   shoeAmountLabel = NULL;
   hourglass_sprite = NULL;
   game_over_sprite = NULL;
   goldEndLabel = NULL;
   distanceEndLabel = NULL;

}

//-----------------------------------------------------------------------------

void MainGame::Init()
{
   Inherited::Init();

   

   hourglass_sprite = frog_new Sprite();
   hourglass_sprite->Init("GUI/MainGame/Sprites", "Hourglass");

   game_over_sprite = frog_new Sprite();
   game_over_sprite->Init("GUI/MainGame/Sprites", "GameOver");

   Background = frog_new Tile();
   Background->Init(theScreen->WidthGet() / 2.0f, theScreen->HeightGet() / 2.0f, 0.07f, theImages->Load("Backgrounds/CartoonSky"), false, true);
   
   Background1 = frog_new Tile();
   Background1->Init(theScreen->WidthGet() / 2.0f + 1366.0f, theScreen->HeightGet() / 2.0f, 0.07f, theImages->Load("Backgrounds/CartoonSky"), false, true);
   
   tile = frog_new Tile();
   tile->Init(theScreen->WidthGet() / 2.0f, theScreen->HeightGet() - 50.0f, (0.4f), theImages->Load("Backgrounds/CartoonGround"), false, true);

   tile1 = frog_new Tile();
   tile1->Init((theScreen->WidthGet() / 2.0f + 1364.0f), theScreen->HeightGet() - 50.0f, (0.4f), theImages->Load("Backgrounds/CartoonGround"), false, true);

   tile2 = frog_new Tile();
   tile2->Init((theScreen->WidthGet() / 2.0f) + 1500.0f, theScreen->HeightGet() - 50.0f, (0.4f), theImages->Load("Backgrounds/CartoonGround"), false, true);

   tile3 = frog_new Tile();
   tile3->Init((theScreen->WidthGet() / 2.0f) + 800.0f, theScreen->HeightGet() - 50.0f, (0.4f), theImages->Load("Backgrounds/CartoonGround"), false, true);

   player = frog_new Player();
   player->Init();

   tiles[0] = tile;
   tiles[1] = tile1;
   tiles[2] = tile2;
   tiles[3] = tile3;
   
   float coinH = tile->getPosition().y - (tile->getHeight() / 2.0f) - 20.0f;

   for (int i = 0; i < 10; i++){
	   coins[i] = frog_new Tile();
	   coins[i]->Init(136.6f * (i + 1), tile->getPosition().y - (tile->getHeight() / 2.0f) - 20.0f, 0.4f, theImages->Load("PowerUps/coin1"), true, true);
	   coins[i]->coinHeight = coinH;
   }
   // Load obstacles 
   jObst[0] = frog_new Tile();
   jObst[0]->Init(0.0f, 0.0f, .4f, theImages->Load("Hazards/Box"), false, true);
   jObst[0]->obstacle = true;
   jObst[0]->position.y = tile->position.y - (tile->getHeight() / 2.0f);// -jObst[0]->getHeight() + 20.0f;

   jObst[1] = frog_new Tile();
   jObst[1]->Init(0.0f, 0.0f, .4f, theImages->Load("Hazards/CobblestoneSpike"), false, false);
   jObst[1]->obstacle = true;
   jObst[1]->position.y = tile->position.y - (tile->getHeight() / 2.0f);

   jObst[2] = frog_new Tile();
   jObst[2]->Init(0.0f, 0.0f, .4f, theImages->Load("Hazards/Fire"), false, false);
   jObst[2]->obstacle = true;
   jObst[2]->position.y = tile->position.y - (tile->getHeight() / 2.0f);

   jObst[3] = frog_new Tile();
   jObst[3]->Init(0.0f, 0.0f, .4f, theImages->Load("Hazards/Spike"), false, false);
   jObst[3]->obstacle = true;
   jObst[3]->position.y = tile->position.y - (tile->getHeight() / 2.0f);

   jObst[4] = frog_new Tile();
   jObst[4]->Init(0.0f, 0.0f, .4f, theImages->Load("Hazards/StoneBrickSpikes"), false, false);
   jObst[4]->obstacle = true;
   jObst[4]->position.y = tile->position.y - (tile->getHeight() / 2.0f);

   jObst[5] = frog_new Tile();
   jObst[5]->Init(0.0f, 0.0f, .4f, theImages->Load("Hazards/Stump"), false, false);
   jObst[5]->obstacle = true;
   jObst[5]->position.y = tile->position.y - (tile->getHeight() / 2.0f);

   jObst[6] = frog_new Tile();
   jObst[6]->Init(0.0f, 0.0f, .4f, theImages->Load("Hazards/basilisk"), false, false);
   jObst[6]->obstacle = true;
   jObst[6]->position.y = tile->position.y - (tile->getHeight() / 2.0f);

   minDistNextCoinPlaced = 0;
   minDistNextObstPlaced = 50;

   dist = 0;
   goldCount = 0;
   multiplyCount = 0;
   magnetCount = 0;
   shoeCount = 0;

   /// Load sounds
   sounds.Init();
   sounds.Reserve(SOUND_COUNT);
   for (int i = 0; i < SOUND_COUNT; i++)
	   sounds.Add(theSounds->Load(SOUND_PATHS[i]));








}

//-----------------------------------------------------------------------------
void MainGame::OnGUILayerInit(LayerWidget*)
{
	// Set up the GUI callbacks.
	PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Pause", OnPauseClick);
	PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Shoe", OnShoeClick);
	PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Multiply", OnMultiplyClick);
	PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Magnet", OnMagnetClick);
	PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".MainMenu", OnMenuClick);
	PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Rerun", OnRerunClick);


	goldLabel = dynamic_cast<NumberLabelWidget*>(theGUI->WidgetGetByPath(GUI_LAYER_NAME ".Gold"));
	distanceLabel = dynamic_cast<NumberLabelWidget*>(theGUI->WidgetGetByPath(GUI_LAYER_NAME ".Distance"));
	multiplyAmountLabel = dynamic_cast<NumberLabelWidget*>(theGUI->WidgetGetByPath(GUI_LAYER_NAME ".MultiplyAmount"));
	magnetAmountLabel = dynamic_cast<NumberLabelWidget*>(theGUI->WidgetGetByPath(GUI_LAYER_NAME ".MagnetAmount"));
	shoeAmountLabel = dynamic_cast<NumberLabelWidget*>(theGUI->WidgetGetByPath(GUI_LAYER_NAME ".ShoeAmount"));
	goldEndLabel = dynamic_cast<NumberLabelWidget*>(theGUI->WidgetGetByPath(GUI_LAYER_NAME ".GoldEnd"));
	distanceEndLabel = dynamic_cast<NumberLabelWidget*>(theGUI->WidgetGetByPath(GUI_LAYER_NAME ".DistanceEnd"));


}

void MainGame::Deinit()
{
	goldLabel = NULL;
	multiplyAmountLabel = NULL;
	magnetAmountLabel = NULL;
	shoeAmountLabel = NULL;
	goldEndLabel = NULL;

   if(player)
   {
	   player->Deinit();
	   frog_delete player;
	   player = NULL;
   }
   if (Background)
   {
	   Background->Deinit();
	   frog_delete Background;
	   Background = NULL;
   }
   if (Background1)
   {
	   Background1->Deinit();
	   frog_delete Background1;
	   Background1 = NULL;
   }
   if (tile)
   {
	   tile->Deinit();
	   frog_delete tile;
	   tile = NULL;
   }
   if (tile1)
   {
	   tile1->Deinit();
	   frog_delete tile1;
	   tile1 = NULL;
   }
   if (tile2)
   {
	   tile2->Deinit();
	   frog_delete tile2;
	   tile2 = NULL;
   }
   if (tile3)
   {
	   tile3->Deinit();
	   frog_delete tile3;
	   tile3 = NULL;
   }
   if (hourglass_sprite)
   {
	   hourglass_sprite->Deinit();
	   frog_delete hourglass_sprite;
	   hourglass_sprite = NULL;
   }
   if (game_over_sprite)
   {
	   game_over_sprite->Deinit();
	   frog_delete game_over_sprite;
	   game_over_sprite = NULL;
   }
   if (coins){
	   for (int i = 0; i < 10; i++){
		   coins[i]->Deinit();
		   frog_delete coins[i];
		   coins[i] = NULL;
	   }
   }
   if (jObst){
	   for (int i = 0; i < 7; i++){
		   jObst[i]->Deinit();
		   frog_delete jObst[i];
		   jObst[i] = NULL;
	   }
   }

   for (int i = 0; i < sounds.SizeGet(); i++)
	   theSounds->Unload(sounds[i]);
   sounds.Deinit();

   Inherited::Deinit();
}

//-----------------------------------------------------------------------------

const char* MainGame::GUILayerNameGet()
{
   return GUI_LAYER_NAME;
}

//-----------------------------------------------------------------------------
void MainGame::OnPauseClick(PressButtonWidget*, void*)
{
	//if (player->paused):


	
}

void MainGame::OnShoeClick(PressButtonWidget*, void*)
{

}

void MainGame::OnMultiplyClick(PressButtonWidget*, void*)
{

}

void MainGame::OnMagnetClick(PressButtonWidget*, void*)
{

}

void MainGame::OnMenuClick(PressButtonWidget*, void*)
{
	theMainGame->StateChangeTransitionBegin(true);
	theStates->Pop();
	theMainGame->gameOver = false;
	
}

void MainGame::OnRerunClick(PressButtonWidget*, void*)
{	
	theMainGame->Deinit();
	theMainGame->Init();
	theMainGame->gameOver = false;
}

void MainGame::Update()
{
	
	if (!player->paused && !gameOver){
		Inherited::Update();

		unsigned int dt = theClock->LoopDurationGet();

		dist += dt;
		distance = dist / 100;

		if (distanceLabel) {
			distanceLabel->TextFormatFunctionSet(NumberLabelWidget::TextFormatFunctionInteger);
			distanceLabel->ValueSet(distance);
		}

		if (goldLabel) {
			goldLabel->TextFormatFunctionSet(NumberLabelWidget::TextFormatFunctionInteger);
			goldLabel->ValueSet(goldCount);
		}

		if (multiplyAmountLabel) {
			multiplyAmountLabel->TextFormatFunctionSet(NumberLabelWidget::TextFormatFunctionInteger);
			multiplyAmountLabel->ValueSet(multiplyCount);
		}

		if (magnetAmountLabel) {
			magnetAmountLabel->TextFormatFunctionSet(NumberLabelWidget::TextFormatFunctionInteger);
			magnetAmountLabel->ValueSet(magnetCount);
		}

		if (shoeAmountLabel) {
			shoeAmountLabel->TextFormatFunctionSet(NumberLabelWidget::TextFormatFunctionInteger);
			shoeAmountLabel->ValueSet(shoeCount);
		}

		if (goldEndLabel) {
			goldEndLabel->TextFormatFunctionSet(NumberLabelWidget::TextFormatFunctionInteger);
			goldEndLabel->ValueSet(goldCount);
		}

		if (distanceEndLabel) {
			distanceEndLabel->TextFormatFunctionSet(NumberLabelWidget::TextFormatFunctionInteger);
			distanceEndLabel->ValueSet(distance);
		}

		Background->Update(dt);
		Background1->Update(dt);

		for (int i = 0; i < 4; i++){
			tiles[i]->Update(dt);
			tiles[i]->CollisionCheck(player, dt);
		}

		for (int i = 0; i < 2; i++){
			tileSurface = tiles[i]->onSurface;
			if (tileSurface){
				break;
			}
		}
		
		int coinNum = FrogMath::Random(20);
		for (int i = 0; i < 10; i++){
			coins[i]->Update(dt);
			if (distance > minDistNextCoinPlaced){
				if (obstacleJustPlaced && coinsOnObstacle < 3){
					if (!coins[i]->visible){
						coins[i]->Update(dt, true);
						if (coinsOnObstacle == 0){
							coins[i]->position.y = coins[i]->position.y - 120.0f;
							coins[i]->position.x = obstacleJustPlacedX - 150.0f;
						}
						else if (coinsOnObstacle == 1){
							coins[i]->position.y = coins[i]->position.y - 160.0f;
							coins[i]->position.x = obstacleJustPlacedX - 120.0f;
						}
						else if (coinsOnObstacle == 2){
							coins[i]->position.y = coins[i]->position.y - 120.0f;
							coins[i]->position.x = obstacleJustPlacedX - 90.0f;
						}
						
						minDistNextCoinPlaced = distance + .5f;
						coinsOnObstacle++;
						if (coinsOnObstacle > 2){
							obstacleJustPlaced = false;
						}
					}
				}
				else if (i == coinNum){
					if (!coins[i]->visible){
						coins[i]->Update(dt, true);
						minDistNextCoinPlaced = distance + 10.0f;
					}
				}
				else if (coinNum > 9){
					minDistNextCoinPlaced = distance + 5.0f;
				}
			}
			coins[i]->CollisionCheck(player, dt);
			if (coins[i]->goldScored){
				goldCount += 1;
				sounds[SOUND_COIN]->Play(NULL, false);
				coins[i]->goldScored = false;
			}
		}

		int math = FrogMath::Random(10);
		for (int i = 0; i < 7; i++){
			if (i == math){
				if (distance > minDistNextObstPlaced && !jObst[i]->visible){
					jObst[i]->Update(dt, false, true);
					obstacleJustPlaced = true;
					obstacleJustPlacedX = jObst[i]->position.x;
					coinsOnObstacle = 0;
					if (distance < 200){
						minDistNextObstPlaced = distance + 80;
					}
					else if (distance < 400){
						minDistNextObstPlaced = distance + 60;
					}
					else if (distance < 600){
						minDistNextObstPlaced = distance + 40;
					}
					else if (distance < 800){
						minDistNextObstPlaced = distance + 20;
					}
					else{
						minDistNextObstPlaced = distance + 10;
					}
				}

			}
			else if (math > 6){
				if (distance > minDistNextObstPlaced){
					if (distance < 600){
						minDistNextObstPlaced = distance + 40;
					}
					else if (distance < 800){
						minDistNextObstPlaced = distance + 20;
					}
					else{
						minDistNextObstPlaced = distance + 10;
					}
				}
			}
			
			jObst[i]->Update(dt);
			jObst[i]->CollisionCheck(player, dt);
			if (jObst[i]->obstacleCollision){
				DebugPrintf("Game Over\n");
				sounds[SOUND_DEATH]->Play(NULL, false);
				Sleep(2000);
				gameOver = true;
				jObst[i]->visible = false;
				jObst[i]->obstacleCollision = false;
			}
			
		}

		player->Update(tileSurface, dt);

		if (!theStates->StateChangeCheck() && theKeyboard->KeyJustPressed(KEY_ESCAPE))
		{
			theMainGame->StateChangeTransitionBegin(true);
			theStates->Pop();
		}

	}

	if (theKeyboard->KeyJustPressed(KEY_SPACE) || theKeyboard->KeyJustPressed(KEY_W)){
			sounds[SOUND_JUMP]->Play(NULL, false);
			//DebugPrintf("jump\n");
	}
	else if (player->paused){
		player->paused = false;
		//Sleep(500);
		dist = 0;
	}
	
	if (theSounds->MusicStoppedCheck())
		theSounds->MusicPlay("Sounds/bmusic");

}

	//-----------------------------------------------------------------------------

void MainGame::Draw()
{
	Background->Draw();
	Background1->Draw();

	tile3->Draw();
	tile2->Draw();
	tile->Draw();
	tile1->Draw();
	
	player->Draw();
	for (int i = 0; i < 10; i++){
		coins[i]->Draw();
	}

	for (int i = 0; i < 7; i++){
		jObst[i]->Draw();
	}

	Point2F hourglass_pos = { 290.0f, -5.0f };
	hourglass_sprite->PositionSet(hourglass_pos);
	hourglass_sprite->Draw();

	if (gameOver){
		Point2F game_over_pos = { 200.0f, 100.0f };
		game_over_sprite->PositionSet(game_over_pos);
		game_over_sprite->Draw();
		distanceEndLabel->VisibleSet(true);
		goldEndLabel->VisibleSet(true);
		PressButtonWidget::VisibleSet(GUI_LAYER_NAME ".Rerun", true);
		PressButtonWidget::VisibleSet(GUI_LAYER_NAME ".MainMenu", true);
	}
	
	
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
	//DebugPrintf("starting height: %f\n", position.y);
	mysprite->PositionSet(position);
	mysprite->AnimationSet("Player");
	onSurface = false;
	velocity = Point2F::Create(0.0f, 0.0f);
	accel = Point2F::Create(0.0f, 0.0f);


	//coinn.setBuffer(buffer);

	//DebugPrintf("Loaded audio");

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
	   paused = true;
	   position = Point2F::Create(theScreen->SizeGet() / 2.0f);
	   position.y = theScreen->HeightGet() / 4.0f;
	   
   }
   if (velocity.y > 2.0f){
	   velocity.y = 2.0f;
   }
   
   
   /*
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
   */
   if (theKeyboard->KeyJustPressed(KEY_SPACE)){
	   if (onSurface){
		   velocity.y = -2.5f;
	   }
   }

   if (theKeyboard->KeyJustPressed(KEY_W)){
	   if (onSurface){
		   velocity.y = -2.5f;
	   }
   }

   /*
   if (theKeyboard->KeyJustPressed(KEY_D)){ //For shooting the arrow
	   if (onSurface){
		   
	   }
   }
   
   if (theKeyboard->KeyJustPressed(KEY_S)){ //For sliding
	   if (onSurface){
		   
	   }
   }

   if (theKeyboard->KeyJustPressed(KEY_A)){ //For slow
	   if (onSurface){
		   
	   }
   }
   */

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

void Player::coinNoise()
{
	//DebugPrintf("Grabbed a coin!\n");

}

Tile::Tile()
{
	image = NULL;
}

//------------------------------------------------------------------------------

void Tile::Init(float xPos, float yPos, float xVel, Image* im, bool isCoin, bool isVisible)
{
	image = im;
	coin = isCoin;
	visible = isVisible;

	height = image->HeightGet();
	width = image->WidthGet();
	height = image->HeightGet();
	width = image->WidthGet();
	//DebugPrintf("height: %f, width: %f\n", height, width);
	position.x = xPos;
	position.y = yPos;
	xVelocity = xVel;

	goldScored = false;
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

void Tile::Update(unsigned int dt, bool newCoin, bool newObstacle)
{
	float dtSeconds = (float)dt / 1000.0f;
	position.x -= xVelocity * float(dt);
	
	if (position.x + width / 2.0f < 0.0f){
		if (coin || obstacle){
			visible = false;
			if (coin){
				position.y = coinHeight;
			}
		}
		if (!(coin || obstacle)){
			position.x = 1363.0f + (width / 2.0f);
			
		}
		if (newObstacle){
			position.x = 1366.0f + (width / 2.0f);
			visible = true;
		}
		if (newCoin){
			position.x = 1366.0f + (width / 2.0f);
			visible = true;
		}
	}
	
}

//------------------------------------------------------------------------------

void Tile::Draw()
{
	if (visible){
		image->Draw(position - (Point2F::Create(image->SizeGet()) / 2.0f));
	}
	
}

//------------------------------------------------------------------------------

void Tile::CollisionCheck(Player* player, unsigned int dt){

	//DebugPrintf("player y pos: %f ,size of sprite y: %f, tile y pos: %f\n", player->position.y, player->mysprite->SizeGet(dt).y, position.y);
	//DebugPrintf("player x pos: %f ,size of sprite x: %f, tile x pos: %f, tile x pos + width: %f\n", player->position.x, player->mysprite->SizeGet(dt).x, position.x, position.x + width);
	//DebugPrintf("sprite height: %f, player height: %f\n", player->mysprite->SizeGet(dt).y, player->height);

	Point2F playerPos = Point2F::Create(player->position);
	Point2F playerSprite = Point2F::Create(player->mysprite->SizeGet(dt));
	Point2F playerVel = Point2F::Create(player->velocity);

	// Checks for collision along the top or bottom of object in question
	if (!coin){
		if (((playerPos.x + playerSprite.x / 2.0f) - 13.0f > position.x - width / 2.0f) && (playerPos.x - playerSprite.x / 2.0f + 13.0f< (position.x + (width / 2.0)))){
			if (playerPos.y + playerSprite.y / 2.0f > position.y - (height / 2.0f) - 1.0f && playerPos.y + playerSprite.y / 2.0f < (position.y - height / 3.0f) && (playerVel.y >= 0)){
				if (visible){
					onSurface = true;
					player->position.y = position.y - height / 2.0f - playerSprite.y / 2.0f;
					if (obstacle){
						obstacleCollision = true;
					}
				}

			}
			else if (playerPos.y - playerSprite.y / 2.0f < position.y + (height / 2.0f) && (playerPos.y - playerSprite.y / 2.0f > position.y + height / 3.0f) && playerVel.y < 0){
				if (visible){
					player->velocity.y = 0.0f;
				}
			}
			else{
				onSurface = false;
			}
		}
		else{
			onSurface = false;
		}

		// Checks for collision along the left or right of object in question
		if ((playerPos.y + playerSprite.y / 2.0f > (position.y - height / 2.0f + 2.0f) && (playerPos.y + playerSprite.y / 2.0f < (position.y + height / 2.0f - 2.0f))) || (playerPos.y - playerSprite.y / 2.0f < (position.y + height / 2.0f - 2.0f) && (playerPos.y - playerSprite.y / 2.0f > (position.y - height / 2.0f + 2.0f)))){
			if ((playerPos.x + playerSprite.x / 2.0f - 13.0f > position.x - width / 2.0f) && (playerPos.x + playerSprite.x / 2.0f - 13.0f < position.x - width / 2.0f + 20.0f)){
				if (visible){
					player->position.x = position.x - width / 2.0f - playerSprite.x / 2.0f + 12.0f;
					if (obstacle){
						obstacleCollision = true;
					}
				}
			}
			else if ((playerPos.x - playerSprite.x / 2.0f + 13.0f < position.x + width / 2.0f) && (playerPos.x - playerSprite.x / 2.0f + 13.0f > position.x + width / 2.0 - 20.0f)){
				if (visible){
					player->position.x = position.x + width / 2.0f + playerSprite.x / 2.0f - 12.0f;
					if (obstacle){
						obstacleCollision = true;
					}
				}
			}
		}
	}
	if (coin && visible){
		if ((playerPos.x + playerSprite.x / 2.0f > position.x - width / 2.0f && playerPos.x - playerSprite.x / 2.0f < position.x - width / 2.0f) && (playerPos.y + playerSprite.y / 2.0f > position.y - height / 2.0f && playerPos.y - playerSprite.y / 2.0f < position.y + height / 2.0f)){
			visible = false;
			goldScored = true;
			player->coinNoise();

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
//FrogMath
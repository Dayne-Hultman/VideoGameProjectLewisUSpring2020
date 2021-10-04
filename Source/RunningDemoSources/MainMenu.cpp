#include "Frog.h"
#include "MainMenu.h"
#include "MainGame.h"
#include "MainUpdate.h"

using namespace Webfoot;

/// Which interface should be shown for this state.
#define GUI_LAYER_NAME "MainMenu"

MainMenu MainMenu::instance;

//-----------------------------------------------------------------------------

void MainMenu::Init()
{
   Inherited::Init();
   exitingGame = false;
}

//-----------------------------------------------------------------------------

void MainMenu::OnGUILayerInit(LayerWidget*)
{
   // Set up the GUI callbacks.
   PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Play", OnPlayClick);
   PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".HowTo", OnHowToClick);
   PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Story", OnStoryClick);
   PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Exit", OnExitClick);
   PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Setting", OnSettingClick);
   PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Information", OnInfoClick);
   PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Shop", OnShopClick);
   PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Achievement", OnAchievementClick);
   PressButtonWidget::OnClickRegister(GUI_LAYER_NAME ".Credit", OnCreditClick);
}

//-----------------------------------------------------------------------------

void MainMenu::Deinit()
{
   if(exitingGame)
      theMainUpdate->Exit();

   Inherited::Deinit();
}

//-----------------------------------------------------------------------------

const char* MainMenu::GUILayerNameGet()
{
   return GUI_LAYER_NAME;
}

//-----------------------------------------------------------------------------

void MainMenu::OnPlayClick(PressButtonWidget*, void*)
{
   if(!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
   {
      theMainMenu->StateChangeTransitionBegin(true);
      theStates->Push(theMainGame);
   }
}

//-----------------------------------------------------------------------------
void MainMenu::OnHowToClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainMenu::OnStoryClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainMenu::OnExitClick(PressButtonWidget*, void*)
{
   if(!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
   {
      theMainMenu->StateChangeTransitionBegin(true);
      theMainMenu->exitingGame = true;
      theStates->Pop();
   }
}

void MainMenu::OnSettingClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{
		
	}
}

void MainMenu::OnInfoClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainMenu::OnShopClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainMenu::OnAchievementClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

void MainMenu::OnCreditClick(PressButtonWidget*, void*)
{
	if (!theStates->StateChangeCheck() && !theMainUpdate->ExitingCheck())
	{

	}
}

//-----------------------------------------------------------------------------

#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Frog.h"
#include "MenuState.h"

namespace Webfoot {

//==============================================================================

/// First menu that the player typically sees.
class MainMenu : public MenuState
{
public:
   typedef MenuState Inherited;

   virtual ~MainMenu() {}
   
   virtual void Init();
   virtual void OnGUILayerInit(LayerWidget* layer);
   virtual void Deinit();

   static MainMenu instance;


protected:
   virtual const char* GUILayerNameGet();

   /// Called when the Play button is clicked.
   static void OnPlayClick(PressButtonWidget*, void*);
   /// Called when the How To button is clicked.
   static void OnHowToClick(PressButtonWidget*, void*);
   /// Called when the Exit button is clicked.
   static void OnExitClick(PressButtonWidget*, void*);
   /// Called when the Settings button is clicked.
   static void OnSettingClick(PressButtonWidget*, void*);
   /// Called when the Information button is clicked.
   static void OnInfoClick(PressButtonWidget*, void*);
   /// Called when the Shop button is clicked.
   static void OnShopClick(PressButtonWidget*, void*);
   /// Called when the Achievement button is clicked.
   static void OnAchievementClick(PressButtonWidget*, void*);
   /// Called when the Credit button is clicked.
   static void OnCreditClick(PressButtonWidget*, void*);
   /// Called when the Story button is clicked.
   static void OnStoryClick(PressButtonWidget*, void*);

   /// True if the Exit button was clicked.
   bool exitingGame;
};

MainMenu* const theMainMenu = &MainMenu::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __MAINMENU_H__

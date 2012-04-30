/*
 Arduino Basic Platform
 Has a screen, LED, rotary encoder, speaker and enclosure.
Copyright Dustin Andrews and/or David Andrews 2012 
 */
#include <LiquidCrystal.h>
#include "MenuEntry.h"
#include "MenuLCD.h"
#include "MenuManager.h"


//Edit your particular harware setup here - See LiquidCrystal documentation for details
const int LCDD7 = 4;
const int LCDD6 = 6;
const int LCDD5 = 7;
const int LCDD4 = 8;
const int LCDE  = 9;
const int LCDRS = 10;

//Now create the MenuLCD and MenuManager classes.
MenuLCD menuLCD( LCDRS, LCDE, LCDD4, LCDD5, LCDD6, LCDD7, 16, 2);
MenuManager menuManager( &menuLCD);//pass the menuLCD object to the menuManager with the & operator.

//when the display is showing user results (e.g. time elapsed), the next "select" should send it back into the menu.
unsigned int isDisplaying = false;



//We would have called LiquidCrystal like this, but instead we let MenuLCD do the below call
//LiquidCrystal lcd(LCDRS, LCDE, LCDD4, LCDD5, LCDD6, LCDD7);
/**************************/


//This function is called during setup to populate the menu with the tree of nodes
//This can be a bit brain-bending to write.  If you draw a tree you want for your menu first
// this code can be a little easier to write.  Add the nodes in a depth-first order for
// the easiest code and least amount of temporary variables. 
//http://en.wikipedia.org/wiki/Depth-first
//
//  This sample code is a simple stopwatch.  Our menu will look like this:
//  Stopwatch
//  |-Start
//  |-Stop
//  |-Reset
//  Timer
//  |-Time
//  | |-1 minute
//  | |-5 minute
//  | |-15 minute
//  |-AutoReset
//  | |-On
//  | |-Off
//  |-Start
//  |-Stop
//  Credits

void StopCallback( char* pMenuText, void *pUserData );
void ResetCallback( char* pMenuText, void *pUserData );

void setupMenus()
{
  
  menuLCD.MenuLCDSetup();  
  //Add nodes via a depth-first traversal order
  MenuEntry * p_menuEntryRoot;
  //Add root node
  //MenuEntry( char * menuText, void * userData/*=0*/, MENU_ACTION_CALLBACK_FUNC func);
  p_menuEntryRoot = new MenuEntry("Stopwatch", NULL, NULL);
  menuManager.addMenuRoot( p_menuEntryRoot );
  //add stopwatch children
  menuManager.addChild( new MenuEntry("Stopwatch Start", NULL, StartCallback) );
  menuManager.addChild( new MenuEntry("Stopwatch Stop", NULL, StopCallback ) );
  menuManager.addChild( new MenuEntry("Reset", NULL, ResetCallback) );
  
  menuManager.addSibling( new MenuEntry("Timer", NULL, NULL ) );
  //Now we want to select the "Timer" entry so we can add children under that node
  //"Timer" is one down from "Stopwatch", so we issue the down command
  menuManager.MenuDown();
  menuManager.addChild( new MenuEntry("Time", NULL, NULL ) );
  //now move down to the "Time" node to add children by selecting the "Timer" node 
  menuManager.MenuSelect(); 
  menuManager.addChild( new MenuEntry("1 minute", NULL, NULL) );
  menuManager.addChild( new MenuEntry("5 minute", NULL, NULL) );
  menuManager.addChild( new MenuEntry("15 minute", NULL, NULL) );
  //Add "time"'s sibling "AutoReset" and select it
  menuManager.addSibling( new MenuEntry( "AutoReset", NULL, NULL) );
  menuManager.MenuDown();
  //Add "AutoReset"'s children
  menuManager.addChild( new MenuEntry( "On", NULL, NULL ) );
  menuManager.addChild( new MenuEntry( "Off", NULL, NULL ) );
  //Add timer start and stop
  menuManager.addSibling( new MenuEntry( "Countdown Start", NULL, NULL) );
  menuManager.addSibling( new MenuEntry( "Countdown Stop", NULL, NULL) );
  
  menuManager.MenuUp();
  menuManager.addSibling( new MenuEntry( "Credits", NULL, NULL) );
  
  menuManager.SelectRoot();
  menuManager.DrawMenu();
}



void setup() 
{           
  Serial.begin(115200);
  Serial.print("Ready.");
  setupMenus();
}



void loop() 
{
  //The example shows using bytes on the serial port to move the menu. You can hook up your buttons or other controls.
  char incomingByte = 0;
  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read();
  }
  switch( incomingByte )
  {
    case 'u':
      menuManager.MenuUp();
      break;
    case 'd':
      menuManager.MenuDown();
      break;
    case 's':
      if( isDisplaying )
      {
        isDisplaying = false;
        menuManager.DrawMenu();
      }
      else
      {
        menuManager.MenuSelect();
      }
      break;
    case 'b':
      menuManager.MenuBack();
      break;
    default:
      break;
  }
}

unsigned long startMillis = 0;
unsigned long stopMillis = 0;

//This is a sample callback funtion for when a menu item with no children (aka command) is selected
void StartCallback( char* pMenuText, void *pUserData )
{
  startMillis = millis();
  char *pTextLines[2] = {"Clock Started", "" };
  menuLCD.MenuLCDPrint( pTextLines, 2, 3 );
  isDisplaying = true;  
}


//This is a sample callback funtion for when a menu item with no children (aka command) is selected
void StopCallback( char* pMenuText, void *pUserData )
{
  stopMillis = millis();
  
  char strSeconds[50];
  dtostrf( ((float)(stopMillis-startMillis))/1000, 1, 2, strSeconds );
  char *pTextLines[2] = {"Elapsed Time", strSeconds };
  menuLCD.MenuLCDPrint( pTextLines, 2, 3 );
  isDisplaying = true;
}  
  
//This is a sample callback funtion for when a menu item with no children (aka command) is selected
void ResetCallback( char* pMenuText, void *pUserData )
{
  startMillis = 0;
  stopMillis = 0;
  char *pTextLines[2] = {"Clock reset", "" };
  menuLCD.MenuLCDPrint( pTextLines, 2, 3 );

}

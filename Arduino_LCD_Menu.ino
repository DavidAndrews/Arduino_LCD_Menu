/*
 Arduino_LCD_Menu Library
Copyright Dustin Andrews, David Andrews 2012 
Licensed under the follwing license:

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
in the documentation and/or other materials provided with the distribution. 
The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
//  |-Set Time
//  |-AutoReset
//  | |-On
//  | |-Off
//  |-Start
//  |-Stop
//  Credits

//void WatchStopCallback( char* pMenuText, void *pUserData );
//void WatchResetCallback( char* pMenuText, void *pUserData );
//void SetTimeCallback( char* pMenuText, void *pUserData );
int timerTime = 0;
unsigned int fTimerRunning = false;
unsigned int timerTarget = 0;
unsigned int fAutoReset = false;

void setupMenus()
{
  
  menuLCD.MenuLCDSetup();  
  //Add nodes via a depth-first traversal order
  MenuEntry * p_menuEntryRoot;
  //Add root node
  //MenuEntry( char * menuText, void * userData/*=0*/, MENU_ACTION_CALLBACK_FUNC func);
  p_menuEntryRoot = new MenuEntry("Stopwatch", NULL, NULL);
  menuManager.addMenuRoot( p_menuEntryRoot );
  menuManager.addChild( new MenuEntry("Stopwatch Start", NULL, WatchStartCallback) );
  menuManager.addChild( new MenuEntry("Stopwatch Stop", NULL, WatchStopCallback ) );
  menuManager.addChild( new MenuEntry("Reset", NULL, WatchResetCallback) );
  
  menuManager.addSibling( new MenuEntry("Timer", NULL, NULL ) );
  //Now we want to select the "Timer" entry so we can add children under that node
  //"Timer" is one down from "Stopwatch", so we issue the down command
  menuManager.MenuDown();
  menuManager.addChild( new MenuEntry("Set Time", NULL, SetTimeCallback ) );
  //now move down to the "Time" node to add children by selecting the "Timer" node 
  menuManager.MenuSelect(); 
  //Add "time"'s sibling "AutoReset" and select it
  menuManager.addSibling( new MenuEntry( "AutoReset", NULL, NULL) );
  menuManager.MenuDown();
  //Add "AutoReset"'s children
  menuManager.addChild( new MenuEntry( "On", NULL, NULL ) );
  menuManager.addChild( new MenuEntry( "Off", NULL, NULL ) );
  //Add timer start and stop
  menuManager.addSibling( new MenuEntry( "Countdown Start", NULL, TimerStartCallback) );
  menuManager.addSibling( new MenuEntry( "Countdown Stop", NULL, TimerStopCallback) );
  
  menuManager.SelectRoot();
  menuManager.addSibling( new MenuEntry( "Credits", NULL, CreditsCallback) );
  
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
      menuManager.DoMenuAction( MENU_ACTION_UP );
      break;
    case 'd':
      menuManager.DoMenuAction( MENU_ACTION_DOWN );
      break;
    case 's':
      if( isDisplaying )
      {
        isDisplaying = false;
        menuManager.DrawMenu();
      }
      else
      {
      menuManager.DoMenuAction( MENU_ACTION_SELECT );
      }
      break;
    case 'b':
      menuManager.DoMenuAction( MENU_ACTION_BACK );
      break;
    default:
      break;
  }
  if( fTimerRunning && timerTarget < millis())
  {
    Serial.println("Timer Goes Off HERE!");
    if( fAutoReset)
    {
      timerTarget = millis() + timerTime;
    }
    else
    {
      fTimerRunning = false;
    }
  }
}
unsigned long startMillis = 0;
unsigned long stopMillis = 0;

//This is a sample callback funtion for when a menu item with no children (aka command) is selected
void WatchStartCallback( char* pMenuText, void *pUserData )
{
  startMillis = millis();
  char *pTextLines[2] = {"Clock Started", "" };
  menuLCD.PrintMenu( pTextLines, 2, 3 );
  isDisplaying = true;  
}


//This is a sample callback funtion for when a menu item with no children (aka command) is selected
void WatchStopCallback( char* pMenuText, void *pUserData )
{
  stopMillis = millis();
  
  char strSeconds[50];
  dtostrf( ((float)(stopMillis-startMillis))/1000, 1, 2, strSeconds );
  char *pTextLines[2] = {"Elapsed Time", strSeconds };
  menuLCD.PrintMenu( pTextLines, 2, 3 );
  isDisplaying = true;
}  
  
//This is a sample callback funtion for when a menu item with no children (aka command) is selected
void WatchResetCallback( char* pMenuText, void *pUserData )
{
  startMillis = 0;
  stopMillis = 0;
  char *pTextLines[2] = {"Clock reset", "" };
  menuLCD.PrintMenu( pTextLines, 2, 3 );
}

void SetTimeCallback( char* pMenuText, void *pUserData )
{
  char *pLabel = "Input a Number";
  menuManager.DoIntInput( 1,1000,10,5, &pLabel, 1, &timerTime );
}
//This is a sample callback funtion for when a menu item with no children (aka command) is selected
void TimerStartCallback( char* pMenuText, void *pUserData )
{
  timerTarget = millis() + (timerTime * 1000);//This is buggy- doesn't handle wrap-around of the millis output.  Too bad :(  
  Serial.print( "timer target = ");
  Serial.println( timerTarget );
  Serial.print( "time = " );
  Serial.println( millis());
  fTimerRunning = true;
  char strSeconds[50];
  itoa( timerTime, strSeconds, 10 );
  char *pTextLines[2] = {"Timer started for ", strSeconds };
  menuLCD.PrintMenu( pTextLines, 2, 3 );
  isDisplaying = true;
}  

unsigned int StopMillis = 0;

//This is a sample callback funtion for when a menu item with no children (aka command) is selected
void TimerStopCallback( char* pMenuText, void *pUserData )
{
  fTimerRunning = false;
}  

void CreditsCallback( char* pMenuText, void *pUserData )
{
  char *pTextLines[2] = {"David Andrews ", "Dustin Andrews" };
  menuLCD.PrintMenu( pTextLines, 2, 1 );
  isDisplaying = true;
}  


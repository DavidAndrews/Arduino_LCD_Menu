/*
 Arduino Basic Platform
 Has a screen, LED, rotary encoder, speaker and enclosure.
Copyright Dustin Andrews and/or David Andrews 2012 
 */
#include <LiquidCrystal.h>
#include <Encoder.h>
#include <Bounce.h>
#include "MenuEntry.h"
#include "MenuLCD.h"
#include "MenuManager.h"

//A0, A3, A4, A5, A6, A7, D0, D1, D3
const int ROTARY1 = A1;
const int HALL = A2;
const int ROTARY0 = 2;
const int LCDD7 = 4;
const int SPEAKER = 5;
const int LCDD6 = 6;
const int LCDD5 = 7;
const int LCDD4 = 8;
const int LCDE  = 9;
const int LCDRS = 10;
const int LCDV0 = 11;
const int CLICK = 12;
const int LEDPIN = 13;
MenuLCD menuLCD( LCDRS, LCDE, LCDD4, LCDD5, LCDD6, LCDD7, 16, 2);
MenuManager menuManager( &menuLCD);

/* setup hardware devices */
//LiquidCrystal lcd(LCDRS, LCDE, LCDD4, LCDD5, LCDD6, LCDD7);
Encoder rotary(ROTARY0, ROTARY1);
Bounce clicker(CLICK, 5);
long rotaryPos = 0;
long rotaryOld = 0;
/**************************/

int brightness = 77;
bool ledOn = false;
unsigned int freq = 1000;

void StartCallback( char* pMenuText )
{
  Serial.println( pMenuText );
}

void setupMenus()
{
  //Add nodes via a depth-first traversal order
  //First create temp variables for your deepest menu level
  MenuEntry * p_menuEntryRoot;
  MenuEntry * p_menuEntries[4];
  //Add root node
  p_menuEntryRoot = new MenuEntry("Settings", NULL, NULL);
  //Start dive down the first leg of the tree
  p_menuEntryRoot->addChild( p_menuEntries[0] = new MenuEntry("My Settings", NULL, NULL) );
  p_menuEntries[0]->addChild( p_menuEntries[1] = new MenuEntry("Unit Settings", NULL, NULL) );
  p_menuEntries[1]->addChild( p_menuEntries[2] = new MenuEntry("Power Options", NULL, NULL) );
  p_menuEntries[2]->addChild( new MenuEntry("Reset", NULL, NULL) );
  
  p_menuEntryRoot->addSibling( p_menuEntries[0] = new MenuEntry("Timers", NULL, NULL) );
  p_menuEntries[0]->addChild( p_menuEntries[1] = new MenuEntry("Countdown Timer", NULL, NULL) );
  p_menuEntries[1]->addSibling( p_menuEntries[2] = new MenuEntry("Stopwatch", NULL, NULL) );
  p_menuEntries[2]->addChild( p_menuEntries[3] = new MenuEntry("Start", NULL, StartCallback) );
  p_menuEntries[1]->addSibling( p_menuEntries[2] = new MenuEntry("Round Timer", NULL, NULL) );
  p_menuEntryRoot->addSibling( p_menuEntries[0] = new MenuEntry("Download", NULL, NULL ));
  p_menuEntries[0]->addSibling( p_menuEntries[1] = new MenuEntry("Export Times", NULL, NULL) );
  p_menuEntryRoot->addSibling( new MenuEntry("Power Off", NULL, NULL ));
  
  menuLCD.MenuLCDSetup();
  menuManager.addMenuRoot( p_menuEntryRoot );
  menuManager.DrawMenu();
}



void setup() 
{           
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(LEDPIN, OUTPUT);
  pinMode(LCDV0, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  digitalWrite(CLICK, HIGH);//set pullup resistor.
  clicker.write(HIGH);
  analogWrite(LCDV0, brightness); 
  Serial.begin(115200);
  Serial.print("Ready.");
  setupMenus();

}

int oldClick = 0;
bool buttonDown = false;
bool ignoreRotate = false;
void loop() 
{
 static long oldPos = -999;
 analogWrite(LCDV0, brightness);
 rotaryPos = rotary.read();
 if(oldPos != rotaryPos)
 {
    Serial.println(rotaryPos);
    oldPos = rotaryPos;
 }
 
 clicker.update();

 if(clicker.fallingEdge())
 {
   buttonDown = true;
 }
 
 if(clicker.risingEdge())
 {
   buttonDown = false;
   if(!ignoreRotate)
   {
     menuManager.MenuSelect();
   }
   ignoreRotate = false;
   rotaryPos = rotaryOld;
 }
 digitalWrite(LEDPIN, ledOn);

 if(abs(rotaryPos - rotaryOld)>2)
 {
   if( buttonDown )
   {
     menuManager.MenuBack();
     rotaryPos = rotaryOld;
     ignoreRotate = true;
   }
   else
   {
     if( rotaryPos < rotaryOld )
     {
       menuManager.MenuUp();
     }
     else
     {
       menuManager.MenuDown();
     }
   }
   rotaryOld = rotaryPos;
 }
}

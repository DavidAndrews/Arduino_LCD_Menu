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


#include "MenuEntry.h"
#include "MenuLCD.h"
#include "MenuIntHelper.h"

enum MENU_ACTION { MENU_ACTION_UP, MENU_ACTION_DOWN, MENU_ACTION_SELECT, MENU_ACTION_BACK };

class MenuManager
{
  public:
  MenuManager(MenuLCD* pMenuLCD);
  bool addMenuRoot( MenuEntry * p_menuEntry);
  MenuEntry * getMenuRoot();
  void DrawMenu();
  void DoMenuAction( MENU_ACTION action );
  void MenuUp();
  void MenuDown();
  void MenuSelect();
  void MenuBack();
  void addChild( MenuEntry * p_menuEntry );    
  void addSibling( MenuEntry * p_menuEntry );  
  void SelectRoot();
  void DoIntInput( int iMin, int iMax, int iStart, int iSteps, char **label, int iLabelLines, int *pInt );
  void DrawInputRow( char *pString );

  void WipeMenu( MenuLCD::Direction dir);


  
  private:
  MenuEntry* m_pRootMenuEntry;
  MenuEntry* m_pCurrentMenuEntry;
  MenuLCD* m_pMenuLCD;
  unsigned int m_fDoingIntInput;
  MenuIntHelper *m_pMenuIntHelper;
  int m_iIntLine;
  int *m_pInt;
};




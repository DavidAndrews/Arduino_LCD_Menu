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


#include <arduino.h>

#ifndef MenuEntry_h
#define MenuEntry_h 1


typedef void (*MENU_ACTION_CALLBACK_FUNC)( char * pMenuText, void * pUserData );


//To use these functions, pass a function pointer as the argument to the MenuEntry constructor.
//pUserData should point to an unsigned int that will be set to true or false.
void MenuEntry_BoolTrueCallbackFunc( char * pMenuText, void * pUserData );
void MenuEntry_BoolFalseCallbackFunc( char * pMenuText, void * pUserData );

//Use this callback function for a "Back" menu item for hardware that doesn't include a back button
//pUserData should point to a MenfsuManager object.
void MenuEntry_BackCallbackFunc( char * pMenuText, void * pUserData );

//The MenuEntry class represents one menu item in the overall menu system, such as "Set Time" or "Back"
//The MenuEntry classes point to each other to create a tree of menu items.  You can navigate
// the classes using the get* calls.  MenuManager uses the get* calls to figure out what to draw to the LCD
class MenuEntry
{
  public:
  //Constructor to create each entry.
  MenuEntry( char * menuText, void * userData, MENU_ACTION_CALLBACK_FUNC func);
  //add a child menu item.  They will be kept it the order they are added, from top to bottom.
  bool addChild( MenuEntry* child);
  //Add a menu item as a sibling of this one, at the end of the sibling chain.
  bool addSibling( MenuEntry* sibling);
  //Sets the previous sibling, mostly used during menu creation to notify a new entry where it's
  //previous pointer needs to point.
  void setPrevSibling( MenuEntry* prevSibling);
  //Can set the action call back dynamically. Overrides what was passed to the constructor.
  bool addActionCallback( MENU_ACTION_CALLBACK_FUNC pCallback);
  
  char* getMenuText();
  //Sets the previous sibling, mostly used during menu creation to notify a new entry where it's
  //previous pointer needs to point.
  void setParent( MenuEntry* parent );
  
  MenuEntry *getNextSibling();
  MenuEntry *getPrevSibling();
  MenuEntry *getChild();
  MenuEntry *getParent();
  //This call will call the action callback for use when the menu item is selected.
  //if this menu entry has any children, the callback will not be executed.
  void ExecuteCallback();

  bool isBackEntry() { return (m_callback == MenuEntry_BackCallbackFunc); }
  
  
  private:
  void* m_userData;
  char* m_menuText;
  MenuEntry* m_parent;
  MenuEntry* m_child;
  MenuEntry* m_nextSibling;
  MENU_ACTION_CALLBACK_FUNC m_callback;
  MenuEntry* m_prevSibling;
};



#endif


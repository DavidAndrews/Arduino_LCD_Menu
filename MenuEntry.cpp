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
#include "MenuEntry.h"
#include "MenuManager.h"

MenuEntry::MenuEntry( char * menuText, void * userData, MENU_ACTION_CALLBACK_FUNC func)
{
  m_menuText = strdup(menuText);
  m_userData = userData;
  m_nextSibling = NULL;
  m_prevSibling = NULL;
  m_child = NULL;  
  m_parent = NULL;
  m_callback = func;
}

void MenuEntry::ExecuteCallback()
{
  if( m_callback != NULL )
  {
    m_callback(m_menuText, m_userData);
  }
}

bool MenuEntry::addChild(MenuEntry* child)
{
  child->setParent( this );
  if(m_child != NULL)
  {
    m_child->addSibling( child );
  }
  else
  {
    m_child = child;
  }
  return true;
}

bool MenuEntry::addSibling( MenuEntry* sibling)
{
  sibling->setParent( m_parent );
  if( m_nextSibling != NULL )
  {
    m_nextSibling->addSibling(sibling);
  }
  else
  {
    m_nextSibling = sibling;
    sibling->setPrevSibling( this );
  }
}

void MenuEntry::setPrevSibling( MenuEntry * pPrevSibling)
{
  m_prevSibling = pPrevSibling;
}

char * MenuEntry::getMenuText()
{
  return m_menuText;
}

MenuEntry *MenuEntry::getNextSibling()
{
  return m_nextSibling;
}
MenuEntry *MenuEntry::getPrevSibling()
{
  return m_prevSibling;
}
MenuEntry *MenuEntry::getChild()
{
  return m_child;
}
MenuEntry *MenuEntry::getParent()
{
  return m_parent;
}
void MenuEntry::setParent( MenuEntry * parent)
{
  m_parent = parent;
}

void MenuEntry_BoolTrueCallbackFunc( char * pMenuText, void * pUserData )
{
  *((unsigned int *)pUserData) = true;
}

void MenuEntry_BoolFalseCallbackFunc( char * pMenuText, void * pUserData )
{
  *((unsigned int *)pUserData) = false;
}

void MenuEntry_BackCallbackFunc( char * pMenuText, void * pUserData )
{
  ((MenuManager *)pUserData)->DoMenuAction( MENU_ACTION_BACK );
}



